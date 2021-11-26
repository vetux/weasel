/**
 *  Weasel  -   Gui Process Explorer
 *  Copyright (C) 2021  Julian Zampiccoli
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "weasel/snapshotgenerator.hpp"

#include "procreader.hpp"

unsigned long getRate(float delta,
                      unsigned long current,
                      unsigned long previous) {
    auto diff = current - previous;
    if (diff == 0) {
        return 0;
    } else {
        auto scale = delta == 0 ? 0 : 1 / delta;
        return static_cast<unsigned long>(static_cast<float>(diff) * scale);
    }
}

static float getCpuLoad(const CpuState::Core &core, const CpuState::Core &prev) {
    //https://stackoverflow.com/a/23376195
    //https://unix.stackexchange.com/a/27079
    auto prevIdle = prev.idle + prev.iowait;
    auto idle = core.idle + core.iowait;

    auto prevNonIdle = prev.user + prev.nice + prev.system + prev.irq + prev.softirq + prev.steal;
    auto nonIdle = core.user + core.nice + core.system + core.irq + core.softirq + core.steal;

    auto prevTotal = prevIdle + prevNonIdle;
    auto total = idle + nonIdle;

    auto totald = total - prevTotal;
    auto idled = idle - prevIdle;

    return static_cast<float>(totald - idled) / static_cast<float>(totald);
}

static float getCpuUsage(const Snapshot &snapshot,
                         const Snapshot &oldSnapshot,
                         unsigned long utime,
                         unsigned long stime,
                         unsigned long prevUtime,
                         unsigned long prevStime) {
    //https://github.com/scaidermern/top-processes/blob/2a2b1a84282b0418f51fd145133175dbff6e155a/top_proc.c#L158
    auto total_cpu = snapshot.cpu.cpu.user + snapshot.cpu.cpu.nice + snapshot.cpu.cpu.system + snapshot.cpu.cpu.idle
                     + snapshot.cpu.cpu.iowait + snapshot.cpu.cpu.irq + snapshot.cpu.cpu.softirq +
                     snapshot.cpu.cpu.steal;
    auto prev_total_cpu =
            oldSnapshot.cpu.cpu.user + oldSnapshot.cpu.cpu.nice + oldSnapshot.cpu.cpu.system + oldSnapshot.cpu.cpu.idle
            + oldSnapshot.cpu.cpu.iowait + oldSnapshot.cpu.cpu.irq + oldSnapshot.cpu.cpu.softirq +
            oldSnapshot.cpu.cpu.steal;
    auto total = utime + stime;
    auto prevTotal = prevUtime + prevStime;
    auto cpuTime = total - prevTotal;
    return (static_cast<float>(cpuTime) / static_cast<float>(total_cpu - prev_total_cpu)
            * static_cast<float>(snapshot.cpu.cores.size()));
}

const Snapshot &SnapshotGenerator::next() {
    auto cDelta = std::chrono::high_resolution_clock::now() - lastCreate;
    lastCreate = std::chrono::high_resolution_clock::now();
    float delta = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(cDelta).count()) / 1000.0f;

    Snapshot oldSnapshot = std::move(currentSnapshot);
    currentSnapshot.cpu = ProcReader::readCpuState();
    currentSnapshot.memory = ProcReader::readMemoryState();
    currentSnapshot.sockets = ProcReader::readNetworkState();
    currentSnapshot.processes = ProcReader::readProcesses(currentSnapshot.sockets, processReadFlags, defaultReadFlags);

    for (auto &pair: currentSnapshot.processes) {
        auto &process = pair.second;

        auto it = oldSnapshot.processes.find(pair.first);
        if (it != oldSnapshot.processes.end()) {
            auto &oldProcess = it->second;

            if (process.starttime != oldProcess.starttime) {
                // The process was newly spawned and is not the same process as in the old snapshot.
                currentSnapshot.deadProcesses[process.pid] = oldProcess;
                for (auto &thread: oldProcess.threads) {
                    currentSnapshot.deadThreads[thread.first] = thread.second;
                }
                continue;
            }

            //Set the rate data and check for spawned/dead threads
            for (auto &threadPair: process.threads) {
                auto &thread = threadPair.second;

                auto threadIt = oldProcess.threads.find(threadPair.first);
                if (threadIt != oldProcess.threads.end()) {
                    auto &oldThread = threadIt->second;

                    if (thread.starttime != oldThread.starttime) {
                        // The thread was newly spawned and is not the same thread as in the old snapshot.
                        currentSnapshot.spawnedThreads.insert(thread.tid);
                        currentSnapshot.deadThreads[oldThread.tid] = oldThread;
                        continue;
                    }

                    //Calculate per thread io rates
                    thread.rate_rchar = getRate(delta, thread.rchar, oldThread.rchar);
                    thread.rate_wchar = getRate(delta, thread.wchar, oldThread.wchar);
                    thread.rate_syscr = getRate(delta, thread.syscr, oldThread.syscr);
                    thread.rate_syscw = getRate(delta, thread.syscw, oldThread.syscw);
                    thread.rate_read_bytes = getRate(delta, thread.read_bytes, oldThread.read_bytes);
                    thread.rate_write_bytes = getRate(delta, thread.write_bytes, oldThread.write_bytes);

                    thread.usage = getCpuUsage(currentSnapshot,
                                               oldSnapshot,
                                               thread.utime,
                                               thread.stime,
                                               oldThread.utime,
                                               oldThread.stime);
                } else {
                    // Thread newly spawned
                    currentSnapshot.spawnedThreads.insert(thread.tid);
                }
            }

            //Check for dead threads, threads with different start time are checked above.
            for (auto &threadPair: oldProcess.threads) {
                auto threadIt = process.threads.find(threadPair.first);
                if (threadIt == process.threads.end()) {
                    //Thread died
                    currentSnapshot.deadThreads[threadPair.first] = threadPair.second;
                }
            }

            //Calculate per process total io rates
            process.rate_rchar = getRate(delta, process.rchar, oldProcess.rchar);
            process.rate_wchar = getRate(delta, process.wchar, oldProcess.wchar);
            process.rate_syscr = getRate(delta, process.syscr, oldProcess.syscr);
            process.rate_syscw = getRate(delta, process.syscw, oldProcess.syscw);
            process.rate_read_bytes = getRate(delta, process.read_bytes, oldProcess.read_bytes);
            process.rate_write_bytes = getRate(delta, process.write_bytes, oldProcess.write_bytes);

            //Calculate per process cpu usage
            process.usage = getCpuUsage(currentSnapshot,
                                        oldSnapshot,
                                        process.utime,
                                        process.stime,
                                        oldProcess.utime,
                                        oldProcess.stime);
        } else {
            //Process was newly spawned
            currentSnapshot.spawnedProcesses.insert(process.pid);
            for (auto &thread: process.threads) {
                currentSnapshot.spawnedThreads.insert(thread.first);
            }
        }
    }

    //Find dead processes, processes with identical pid are checked above.
    for (auto &pair: oldSnapshot.processes) {
        auto it = currentSnapshot.processes.find(pair.first);
        if (it == currentSnapshot.processes.end()) {
            //Process is dead.
            currentSnapshot.deadProcesses[pair.first] = pair.second;
            processReadFlags.erase(pair.first);
        }
    }

    //Find dead sockets
    for (auto &pair: oldSnapshot.sockets) {
        auto it = currentSnapshot.sockets.find(pair.first);
        if (it == currentSnapshot.sockets.end()) {
            // Socket is dead
            currentSnapshot.deadSockets[pair.first] = pair.second;
        } else {
            // This check only verifies endpoint, protocol and inode
            // if a socket with the same endpoint protocol and inode is spawned this wont be detected but this should suffice.
            if (pair.second != it->second) {
                // Newly spawned socket with same inode
                currentSnapshot.deadSockets[pair.first] = pair.second;
                currentSnapshot.spawnedSockets.insert(pair.first);
            }
        }
    }

    //Find new sockets
    for (auto &pair: currentSnapshot.sockets) {
        auto it = oldSnapshot.sockets.find(pair.first);
        if (it == oldSnapshot.sockets.end()) {
            // New socket
            currentSnapshot.spawnedSockets.insert(pair.first);
        }
    }

    //Calculate total cpu load
    currentSnapshot.cpu.cpu.load = getCpuLoad(currentSnapshot.cpu.cpu, oldSnapshot.cpu.cpu);

    //Calculate per core cpu load
    for (size_t i = 0; i < currentSnapshot.cpu.cores.size() && i < oldSnapshot.cpu.cores.size(); i++) {
        auto &core = currentSnapshot.cpu.cores.at(i);
        auto &oldCore = oldSnapshot.cpu.cores.at(i);
        core.load = getCpuLoad(core, oldCore);
    }

    currentSnapshot.number = oldSnapshot.number + 1;

    return currentSnapshot;
}