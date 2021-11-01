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

#ifndef WEASEL_SNAPSHOT_HPP
#define WEASEL_SNAPSHOT_HPP

#include <map>

#include "system/cpustate.hpp"
#include "system/memorystate.hpp"
#include "system/process.hpp"

struct Snapshot {
    static Snapshot create();

    CpuState cpu;
    MemoryState memory{};

    std::map<Inode_t, Socket> sockets;
    std::map<Pid_t, Process> processes;
};

#endif //WEASEL_SNAPSHOT_HPP
