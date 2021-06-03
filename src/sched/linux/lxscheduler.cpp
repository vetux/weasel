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

#include "sched/linux/lxscheduler.hpp"

#include <stdexcept>

#include <fstream>
#include <iostream>
#include <filesystem>
#include <cassert>
#include <csignal>
#include <cstring>

#include <unistd.h>

#include <sys/syscall.h>
#include <sys/resource.h>

namespace fs = std::filesystem;

#define LINUX_DIR_PROC "/proc"

#define LINUX_DIR_PROCESS(PID) LINUX_DIR_PROC"/"#PID
#define LINUX_DIR_THREAD(PID, TID) LINUX_DIR_PROCESS(PID)"/task/"#TID

#define LINUX_STATUS_PROCESS(PID) LINUX_DIR_PROCESS(PID)"/status"
#define LINUX_STATUS_THREAD(PID, TID) LINUX_DIR_THREAD(PID,TID)"/status"

#define LINUX_MEMINFO LINUX_DIR_PROC"/meminfo"

pid_t convertPID(int pid) {
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
        if (isAsciiNumber(c))
            return false;
    }
    return true;
}

int tkill(int TID, int signal) {
    syscall(SYS_tkill, TID, signal);
}

Process readProcess(const fs::path &processDir) {
    assert(isPID(processDir.filename()));
    throw std::runtime_error("Not Implemented");
}

Memory readMemory(const std::string &memFile) {
    throw std::runtime_error("Not Implemented");
}

Scheduler *Scheduler::createScheduler() {
    return new LxScheduler();
}

Platform Scheduler::getPlatform() {
    return LINUX;
}

const std::map<int, Process> &LxScheduler::getProcesses() {
    std::vector<fs::directory_entry> entries;
    for (auto &fl : fs::directory_iterator(LINUX_DIR_PROC)) {
        if (isPID(fl.path().filename())) {
            entries.emplace_back(fl);
        }
    }
    processes.clear();
    for (auto &p : entries) {
        auto proc = readProcess(p);
        processes[proc.PID] = proc;
    }
    return processes;
}

const Memory &LxScheduler::getMemory() {
    memory = readMemory(LINUX_MEMINFO);
    return memory;
}

void LxScheduler::signal(const Process &process, Signal signal) {
    int r = kill(convertPID(process.PID), convertSignal(signal));
    if (r == -1) {
        auto err = errno;
        throw std::runtime_error("Failed to send signal: " + std::string(strerror(err)));
    }
}

void LxScheduler::signal(const Thread &thread, Signal signal) {
    int r = tkill(convertPID(thread.TID), convertSignal(signal));
    if (r == -1) {
        auto err = errno;
        throw std::runtime_error("Failed to send signal: " + std::string(strerror(err)));
    }
}

void LxScheduler::setPriority(const Process &process, int priority) {
    int r = setpriority(PRIO_PROCESS, convertPID(process.PID), priority);
    if (r == -1) {
        auto err = errno;
        throw std::runtime_error("Failed to set priority: " + std::string(strerror(err)));
    }
}

void LxScheduler::setPriority(const Thread &thread, int priority) {
    int r = setpriority(PRIO_PROCESS, convertPID(thread.TID), priority);
    if (r == -1) {
        auto err = errno;
        throw std::runtime_error("Failed to set priority: " + std::string(strerror(err)));
    }
}

