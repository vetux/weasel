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

#ifndef WEASEL_SNAPSHOT_HPP
#define WEASEL_SNAPSHOT_HPP

#include <map>
#include <set>

#include "weasel/cpustate.hpp"
#include "weasel/memorystate.hpp"
#include "weasel/process.hpp"

struct Snapshot {
    unsigned long number; //The number of the snapshot, incremented by the generator for each snapshot, wraps around.

    CpuState cpu;
    MemoryState memory{};

    std::map<Inode_t, Socket> sockets;
    std::map<Pid_t, Process> processes;

    // Newly spawned unit keys, the data of which is accessible above.
    std::set<Inode_t> spawnedSockets; // Sockets that were spawned between this snapshot and the previous one.
    std::set<Pid_t> spawnedProcesses; // Processes that were spawned between this snapshot and the previous one.
    std::set<Pid_t> spawnedThreads; // Threads that were spawned between this snapshot and the previous one.

    // Dead units with their values which are not accessible anymore in the current snapshot.
    std::map<Inode_t, Socket> deadSockets; // Sockets that died between this snapshot and the previous one.
    std::map<Pid_t, Process> deadProcesses; // Processes that died between this snapshot and the previous one.
    std::map<Pid_t, Thread> deadThreads; // Threads that died between this snapshot and the previous one.
};

#endif //WEASEL_SNAPSHOT_HPP
