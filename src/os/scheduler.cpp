#include <vector>

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

#include "os/scheduler.hpp"

#include <stdexcept>

#include <fstream>
#include <iostream>
#include <filesystem>
#include <cassert>
#include <csignal>
#include <cstring>
#include <sstream>
#include <algorithm>

#include <unistd.h>

#include <sys/syscall.h>
#include <sys/resource.h>

#include <sched.h>

#include "os/procpath.hpp"

namespace fs = std::filesystem;

pid_t convertPID(Pid_t pid) {
    if (pid > std::numeric_limits<pid_t>::max() || pid < std::numeric_limits<pid_t>::min())
        throw std::runtime_error("Pid value is out of range");
    return static_cast<pid_t>(pid);
}

int convertSignal(Signal signal) {
    switch (signal) {
        case SIGNAL_SIGHUP:
            return SIGHUP;
        case SIGNAL_SIGINT:
            return SIGINT;
        case SIGNAL_SIGQUIT:
            return SIGQUIT;
        case SIGNAL_SIGILL:
            return SIGILL;
        case SIGNAL_SIGABRT:
            return SIGABRT;
        case SIGNAL_SIGFPE:
            return SIGFPE;
        case SIGNAL_SIGKILL:
            return SIGKILL;
        case SIGNAL_SIGSEGV:
            return SIGSEGV;
        case SIGNAL_SIGPIPE:
            return SIGPIPE;
        case SIGNAL_SIGALRM:
            return SIGALRM;
        case SIGNAL_SIGTERM:
            return SIGTERM;
        case SIGNAL_SIGUSR1:
            return SIGUSR1;
        case SIGNAL_SIGUSR2:
            return SIGUSR2;
        case SIGNAL_SIGCHLD:
            return SIGCHLD;
        case SIGNAL_SIGCONT:
            return SIGCONT;
        case SIGNAL_SIGSTOP:
            return SIGSTOP;
        case SIGNAL_SIGTSTP:
            return SIGTSTP;
        case SIGNAL_SIGTTIN:
            return SIGTTIN;
        case SIGNAL_SIGTTOU:
            return SIGTTOU;
        default:
            throw std::runtime_error("Unrecognized signal");
    }
}

bool isAsciiNumber(char c) {
    return c >= '0' && c <= '9';
}

bool isPID(const std::string &name) {
    for (auto &c : name) {
        if (!isAsciiNumber(c))
            return false;
    }
    return true;
}

long tkill(pid_t TID, int signal) {
    return syscall(SYS_tkill, TID, signal);
}

std::string removeDoubleWhiteSpace(const std::string &str) {
    auto ret = str;
    auto space = ret.find("  ");
    while (space != -1) {
        ret.replace(space, 2, " ");
        space = ret.find("  ");
    }
    return ret;
}

std::string removeSurroundingWhiteSpace(const std::string &str) {
    auto ret = str;
    auto space = ret.find(' ');
    while (space == 0) {
        ret.erase(space, 1);
        space = ret.find(' ');
    }
    space = ret.find(' ');
    while (!ret.empty() && space == ret.size() - 1) {
        ret.erase(space, 1);
        space = ret.find(' ');
    }
    return ret;
}

std::string removeTabs(const std::string &str) {
    auto ret = str;
    auto tab = ret.find('\t');
    while (tab != -1) {
        ret.replace(tab, 1, " ");
        tab = ret.find('\t');
    }
    return ret;
}

std::map<std::string, std::string> parseProcFile(const std::string &str) {
    std::istringstream f(str);

    std::map<std::string, std::string> ret;

    std::string line;
    while (std::getline(f, line)) {
        auto dot = line.find(':');
        if (dot == std::string::npos)
            throw std::runtime_error("Invalid proc file format");
        std::string name = line.substr(0, dot);
        std::string value = line.substr(dot + 1);
        value = removeTabs(value);
        value = removeDoubleWhiteSpace(value);
        value = removeSurroundingWhiteSpace(value);
        if (ret.find(name) != ret.end())
            throw std::runtime_error("Invalid proc file double key");
        ret[name] = value;
    }

    return ret;
}

std::string readText(const std::string &filePath) {
    std::string ret;
    std::ifstream stream;

    stream.open(filePath);

    if (stream.fail()) {
        throw std::runtime_error("Failed to read text at " + filePath + " error: " + strerror(errno));
    }

    std::string tmp;
    while (std::getline(stream, tmp)) {
        ret += tmp + "\n";
    }

    return ret;
}

Uid_t getUID(const std::string &uid) {
    std::string t = uid.substr(0, uid.find(' '));
    return stringToUid(t);
}

Mem_t getBytesFromKilobyte(const std::string &mem) {
    std::string tmp;
    tmp = mem.substr(0, mem.find(' '));
    return stringToMem(tmp) * 1000;
}

Thread readThread(const fs::path &statusFile) {
    auto proc = parseProcFile(readText(statusFile));

    Thread ret;

    ret.TID = stringToPid(proc.at("Pid"));
    ret.PID = stringToPid(proc.at("Tgid"));
    ret.UID = getUID(proc.at("Uid"));

    ret.name = proc.at("Name");

    //TODO:Implement memory parsing
    ret.memVirt = 0;
    ret.memRes = 0;
    ret.memShared = 0;

    return ret;
}

std::string readCommand(Pid_t PID) {
    std::ifstream stream;

    std::string filePath = ProcPath::getCommandLineFile(PID);
    stream.open(filePath);

    if (stream.fail()) {
        throw std::runtime_error("Failed to read text at " + filePath + " error: " + strerror(errno));
    }

    std::string ret;
    stream >> ret;
    return ret;
}

Process readProcess(const fs::path &statusFile) {
    auto proc = parseProcFile(readText(statusFile));
    Process ret;

    ret.PID = stringToPid(proc.at("Pid"));
    ret.UID = getUID(proc.at("Uid"));

    ret.name = proc.at("Name");
    ret.command = readCommand(ret.PID);

    ret.priority = getpriority(PRIO_PROCESS, ret.PID);

    ret.parentPID = stringToPid(proc.at("PPid"));

    std::vector<fs::directory_entry> entries;
    for (auto &fl : fs::directory_iterator(ProcPath::getProcessTasksDirectory(ret.PID))) {
        if (isPID(fl.path().filename())) {
            entries.emplace_back(fl);
        }
    }

    for (auto &e : entries) {
        ret.threads.emplace_back(
                readThread(ProcPath::getThreadStatusFile(ret.PID, stringToPid(e.path().filename().string())))
        );
    }

    return ret;
}

Memory readMemory(const fs::path &memFile) {
    auto proc = parseProcFile(readText(memFile));

    Memory ret{};

    ret.total = getBytesFromKilobyte(proc.at("MemTotal"));
    ret.free = getBytesFromKilobyte(proc.at("MemFree"));
    ret.available = getBytesFromKilobyte(proc.at("MemAvailable"));

    return ret;
}

const std::map<Pid_t, Process> &Scheduler::getProcesses() {
    std::vector<fs::directory_entry> entries;
    for (auto &fl : fs::directory_iterator(ProcPath::getProcDirectory())) {
        if (isPID(fl.path().filename())) {
            entries.emplace_back(fl);
        }
    }
    processes.clear();
    for (fs::directory_entry &p : entries) {
        const auto &path = p.path();
        auto pid = stringToPid(path.filename().string());
        auto proc = readProcess(ProcPath::getProcessStatusFile(pid));
        processes[pid] = proc;
    }
    return processes;
}

const Memory &Scheduler::getMemory() {
    memory = readMemory(ProcPath::getMemoryInfoFile());
    return memory;
}

void Scheduler::signal(const Process &process, Signal signal) {
    int r = kill(convertPID(process.PID), convertSignal(signal));
    if (r == -1) {
        auto err = errno;
        throw std::runtime_error("Failed to send signal: " + std::string(strerror(err)));
    }
}

void Scheduler::signal(const Thread &thread, Signal signal) {
    auto r = tkill(convertPID(thread.TID), convertSignal(signal));
    if (r == -1) {
        auto err = errno;
        throw std::runtime_error("Failed to send signal: " + std::string(strerror(err)));
    }
}

void Scheduler::setPriority(const Process &process, int priority) {
    int r = setpriority(PRIO_PROCESS, convertPID(process.PID), priority);
    if (r == -1) {
        auto err = errno;
        throw std::runtime_error("Failed to set priority: " + std::string(strerror(err)));
    }
}

void Scheduler::setPriority(const Thread &thread, int priority) {
    int r = setpriority(PRIO_PROCESS, convertPID(thread.TID), priority);
    if (r == -1) {
        auto err = errno;
        throw std::runtime_error("Failed to set priority: " + std::string(strerror(err)));
    }
}

void Scheduler::setPolicy(const Thread &thread, SchedulingPolicy policy) {
    throw std::runtime_error("Not Implemented");
}

