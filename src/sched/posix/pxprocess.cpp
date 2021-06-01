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

#include "sched/posix/pxprocess.hpp"

#include <stdexcept>

ExecutionState PxProcess::getState() {
    throw std::runtime_error("Not Implemented");
}

std::string PxProcess::getName() {
    throw std::runtime_error("Not Implemented");
}

int PxProcess::getPID() {
    throw std::runtime_error("Not Implemented");
}

float PxProcess::getCPU() {
    throw std::runtime_error("Not Implemented");
}

long PxProcess::getVirtualMemorySize() {
    throw std::runtime_error("Not Implemented");
}

long PxProcess::getReservedMemorySize() {
    throw std::runtime_error("Not Implemented");
}

long PxProcess::getSharedMemorySize() {
    throw std::runtime_error("Not Implemented");
}

int PxProcess::getParent() {
    throw std::runtime_error("Not Implemented");
}

std::vector<Thread *> PxProcess::getThreads() {
    throw std::runtime_error("Not Implemented");
}