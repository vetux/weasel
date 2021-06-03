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

#include "sched/win32/winscheduler.hpp"

#include <stdexcept>

Scheduler *Scheduler::createScheduler() {
    return new WinScheduler();
}

Platform Scheduler::getPlatform() {
    return WIN32;
}

const std::map<int, Process> &WinScheduler::getProcesses() {
    throw std::runtime_error("Not Implemented");
}

const Memory &WinScheduler::getMemory() {
    throw std::runtime_error("Not Implemented");
}

void WinScheduler::signal(const Process &process, Signal signal) {
    throw std::runtime_error("Not Implemented");
}

void WinScheduler::signal(const Thread &thread, Signal signal) {
    throw std::runtime_error("Not Implemented");
}

void WinScheduler::setPriority(const Process &process, int priority) {
    throw std::runtime_error("Not Implemented");
}

void WinScheduler::setPriority(const Thread &thread, int priority) {
    throw std::runtime_error("Not Implemented");
}
