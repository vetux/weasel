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

#include "system/scheduler.hpp"

#include <stdexcept>

#include <filesystem>
#include <csignal>
#include <cstring>
#include <algorithm>

#include <sys/resource.h>

#include <sched.h>

#include "system/procpath.hpp"
#include "system/syscalls.hpp"

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
            throw std::runtime_error("Unrecognized signal value");
    }
}

int convertPolicy(SchedulingPolicy policy) {
    switch (policy) {
        case OTHER:
            return SCHED_OTHER;
        case BATCH:
            return SCHED_BATCH;
        case IDLE:
            return SCHED_IDLE;
        case FIFO:
            return SCHED_FIFO;
        case RR:
            return SCHED_RR;
        case DEADLINE:
            return SCHED_DEADLINE;
        default:
            throw std::runtime_error("Unrecognized policy value");
    }
}

void Scheduler::signal(const Process &process, Signal signal) {
    int r = kill(convertPID(process.pid), convertSignal(signal));
    if (r == -1) {
        auto err = errno;
        throw std::runtime_error("Failed to send signal: " + std::string(strerror(err)));
    }
}

void Scheduler::signal(const Thread &thread, Signal signal) {
    auto r = tkill(convertPID(thread.tid), convertSignal(signal));
    if (r == -1) {
        auto err = errno;
        throw std::runtime_error("Failed to send signal: " + std::string(strerror(err)));
    }
}

void Scheduler::setPriority(const Process &process, int priority) {
    int r = setpriority(PRIO_PROCESS, convertPID(process.pid), priority);
    if (r == -1) {
        auto err = errno;
        throw std::runtime_error("Failed to set priority: " + std::string(strerror(err)));
    }
}

void Scheduler::setPriority(const Thread &thread, int priority) {
    int r = setpriority(PRIO_PROCESS, convertPID(thread.tid), priority);
    if (r == -1) {
        auto err = errno;
        throw std::runtime_error("Failed to set priority: " + std::string(strerror(err)));
    }
}

void Scheduler::setPolicy(const Thread &thread,
                          SchedulingPolicy policy,
                          uint64_t runtime,
                          uint64_t deadline,
                          uint64_t period) {
    sched_attr attr{};
    attr.size = sizeof(struct sched_attr);
    attr.sched_policy = convertPolicy(policy);
    attr.sched_flags = 0;
    attr.sched_nice = thread.priority;
    attr.sched_priority = convertPolicy(policy);
    attr.sched_runtime = runtime;
    attr.sched_deadline = deadline;
    attr.sched_period = period;
    int r = sched_setattr(thread.tid, &attr, 0);
    if (r == -1) {
        auto err = errno;
        throw std::runtime_error("Failed to set policy: " + std::string(strerror(err)));
    }
}

int Scheduler::getPageSize() {
    return getpagesize();
}
