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

#include "system/process.hpp"

#include <limits>
#include <stdexcept>

#include <csignal>
#include <cstring>

#include <sys/resource.h>

static pid_t convertPID(Pid_t pid) {
    if (pid > std::numeric_limits<pid_t>::max() || pid < std::numeric_limits<pid_t>::min())
        throw std::runtime_error("Pid value is out of range");
    return static_cast<pid_t>(pid);
}

static int convertSignal(Signal signal) {
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

const Thread &Process::mainThread() const {
    return threads.at(0);
}

Thread &Process::mainThread() {
    return threads.at(0);
}

void Process::sendSignal(Signal signal) const {
    int r = kill(convertPID(pid), convertSignal(signal));
    if (r == -1) {
        auto err = errno;
        throw std::runtime_error("Failed to send signal: " + std::string(strerror(err)));
    }
}

void Process::setPriority(int priority) const {
    int r = setpriority(PRIO_PROCESS, convertPID(pid), priority);
    if (r == -1) {
        auto err = errno;
        throw std::runtime_error("Failed to set priority: " + std::string(strerror(err)));
    }
}
