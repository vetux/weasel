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

#ifndef WEASEL_THREAD_HPP
#define WEASEL_THREAD_HPP

#include <string>

#include "os/schedulingpolicy.hpp"
#include "os/types.hpp"

struct Thread {
    Pid_t TID; // The TID of this thread
    Pid_t PID; // The PID of the process this thread belongs to
    Uid_t UID; // The UID of the thread owner

    std::string name;

    int priority; // The nice value of the thread
    SchedulingPolicy policy; // The scheduling policy of the thread

    Mem_t memVirt;
    Mem_t memRes;
    Mem_t memShared;
};

#endif //WEASEL_THREAD_HPP
