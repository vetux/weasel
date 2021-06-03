#include <string>
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

#include "sched/linux/lxprocess.hpp"

#include <stdexcept>
#include <fstream>

#include <sys/resource.h>

#include <cerrno>
#include <cstring>

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

void LxProcess::signal(Signal signal) {
    kill(convertPID(pid), convertSignal(signal));
}

void LxProcess::setPriority(int priority) {
    int r = setpriority(PRIO_PROCESS, convertPID(pid), priority);
    if (r == -1) {
        auto err = errno;
        throw std::runtime_error("Failed to set priority: " + std::string(strerror(err)));
    }
}

int LxProcess::getPriority() {
    errno = 0;
    int r = getpriority(PRIO_PROCESS, convertPID(pid));
    if (r == -1 && errno != 0) {
        auto err = errno;
        throw std::runtime_error("Failed to get priority: " + std::string(strerror(err)));
    }
    return r;
}

int LxProcess::getUID() {
    throw std::runtime_error("Not Implemented");
}

bool LxProcess::isPaused() {
    throw std::runtime_error("Not Implemented");
}

std::string LxProcess::getName() {
    throw std::runtime_error("Not Implemented");
}

std::string LxProcess::getDescription() {
    throw std::runtime_error("Not Implemented");
}

std::string LxProcess::getImagePath() {
    throw std::runtime_error("Not Implemented");
}

int LxProcess::getPID() {
    return static_cast<int>(pid);
}

float LxProcess::getCPU() {
    throw std::runtime_error("Not Implemented");
}

long LxProcess::getVirtualMemorySize() {
    throw std::runtime_error("Not Implemented");
}

long LxProcess::getReservedMemorySize() {
    throw std::runtime_error("Not Implemented");
}

long LxProcess::getSharedMemorySize() {
    throw std::runtime_error("Not Implemented");
}

int LxProcess::getParent() {
    throw std::runtime_error("Not Implemented");
}

std::vector<Thread *> LxProcess::getThreads() {
    throw std::runtime_error("Not Implemented");
}
