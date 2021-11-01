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

#include "system/thread.hpp"

#include <stdexcept>
#include <limits>

#include <csignal>
#include <cstring>

#include <sys/resource.h>

#include "system/syscalls.hpp"

static pid_t convertPID(Pid_t pid) {
    if (pid > std::numeric_limits<pid_t>::max() || pid < std::numeric_limits<pid_t>::min())
        throw std::runtime_error("Pid value is out of range");
    return static_cast<pid_t>(pid);
}

static int convertSignal(Thread::Signal signal) {
    switch (signal) {
        case Thread::SIGNAL_SIGHUP:
            return SIGHUP;
        case Thread::SIGNAL_SIGINT:
            return SIGINT;
        case Thread::SIGNAL_SIGQUIT:
            return SIGQUIT;
        case Thread::SIGNAL_SIGILL:
            return SIGILL;
        case Thread::SIGNAL_SIGABRT:
            return SIGABRT;
        case Thread::SIGNAL_SIGFPE:
            return SIGFPE;
        case Thread:: SIGNAL_SIGKILL:
            return SIGKILL;
        case Thread::SIGNAL_SIGSEGV:
            return SIGSEGV;
        case Thread::SIGNAL_SIGPIPE:
            return SIGPIPE;
        case Thread::SIGNAL_SIGALRM:
            return SIGALRM;
        case Thread::SIGNAL_SIGTERM:
            return SIGTERM;
        case Thread::SIGNAL_SIGUSR1:
            return SIGUSR1;
        case Thread::SIGNAL_SIGUSR2:
            return SIGUSR2;
        case Thread::SIGNAL_SIGCHLD:
            return SIGCHLD;
        case Thread::SIGNAL_SIGCONT:
            return SIGCONT;
        case Thread::SIGNAL_SIGSTOP:
            return SIGSTOP;
        case Thread::SIGNAL_SIGTSTP:
            return SIGTSTP;
        case Thread::SIGNAL_SIGTTIN:
            return SIGTTIN;
        case Thread::SIGNAL_SIGTTOU:
            return SIGTTOU;
        default:
            throw std::runtime_error("Unrecognized signal value");
    }
}

static int convertPolicy(Thread::SchedulingPolicy policy) {
    switch (policy) {
        case Thread::OTHER:
            return SCHED_OTHER;
        case Thread::BATCH:
            return SCHED_BATCH;
        case Thread::IDLE:
            return SCHED_IDLE;
        case Thread::FIFO:
            return SCHED_FIFO;
        case Thread::RR:
            return SCHED_RR;
        case Thread::DEADLINE:
            return SCHED_DEADLINE;
        default:
            throw std::runtime_error("Unrecognized policy value");
    }
}

void Thread::sendSignal(Signal s) const {
    auto r = tkill(convertPID(tid), convertSignal(s));
    if (r == -1) {
        auto err = errno;
        throw std::runtime_error("Failed to send signal: " + std::string(strerror(err)));
    }
}

void Thread::setPriority(int p) const {
    int r = setpriority(PRIO_PROCESS, convertPID(tid), p);
    if (r == -1) {
        auto err = errno;
        throw std::runtime_error("Failed to set priority: " + std::string(strerror(err)));
    }
}

void Thread::setPolicy(SchedulingPolicy p, uint64_t runtime, uint64_t deadline, uint64_t period) const {
    sched_attr attr{};
    attr.size = sizeof(struct sched_attr);
    attr.sched_policy = convertPolicy(p);
    attr.sched_flags = 0;
    attr.sched_nice = priority;
    attr.sched_priority = convertPolicy(p);
    attr.sched_runtime = runtime;
    attr.sched_deadline = deadline;
    attr.sched_period = period;
    int r = sched_setattr(tid, &attr, 0);
    if (r == -1) {
        auto err = errno;
        throw std::runtime_error("Failed to set policy: " + std::string(strerror(err)));
    }
}