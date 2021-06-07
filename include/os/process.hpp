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

#ifndef WEASEL_PROCESS_HPP
#define WEASEL_PROCESS_HPP

#include <string>
#include <vector>

#include "os/thread.hpp"
#include "os/types.hpp"

struct Process {
    Pid_t PID;
    Uid_t UID;

    std::string name;
    std::string command;

    int priority; // The process nice value

    Mem_t memVirt;
    Mem_t memRes;
    Mem_t memShared;

    Pid_t parentPID; // The parent process PID or -1 if no parent.

    std::vector<Thread> threads;
};

#endif //WEASEL_PROCESS_HPP
