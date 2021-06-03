#include <string>

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

#include "sched/linux/lxthread.hpp"

#include <stdexcept>

void LxThread::signal(Signal signal) {
    throw std::runtime_error("Not Implemented");
}


bool LxThread::isPaused() {
    throw std::runtime_error("Not Implemented");
}

int LxThread::getUID() {
    throw std::runtime_error("Not Implemented");
}

std::string LxThread::getName() {
    throw std::runtime_error("Not Implemented");
}

int LxThread::getTID() {
    throw std::runtime_error("Not Implemented");
}

float LxThread::getCPU() {
    throw std::runtime_error("Not Implemented");
}

long LxThread::getVirtualMemorySize() {
    throw std::runtime_error("Not Implemented");
}

long LxThread::getReservedMemorySize() {
    throw std::runtime_error("Not Implemented");
}

long LxThread::getSharedMemorySize() {
    throw std::runtime_error("Not Implemented");
}

int LxThread::getProcess() {
    throw std::runtime_error("Not Implemented");
}