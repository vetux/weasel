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

#ifndef WEASEL_PROCREADER_HPP
#define WEASEL_PROCREADER_HPP

#include <filesystem>
#include <stdexcept>
#include <map>
#include <set>

#include "weasel/thread.hpp"
#include "weasel/process.hpp"
#include "weasel/cpustate.hpp"
#include "weasel/memorystate.hpp"
#include "weasel/processreadflags.hpp"

namespace ProcReader {
    CpuState readCpuState();

    MemoryState readMemoryState();

    std::map<Inode_t, Socket> readNetworkState();

    /**
     * @return The currently active processes from /proc/PID
     */
    std::map<Pid_t, Process> readProcesses(const std::map<Inode_t, Socket> &netStat,
                                           const std::map<Pid_t, ProcessReadFlags> &flags,
                                           ProcessReadFlags defaultFlags);

    /**
     * @param pid The pid of the process to read.
     * @return The data for the process with pid
     */
    Process readProcess(Pid_t pid, const std::map<Inode_t, Socket> &netStat, ProcessReadFlags flags);

    /**
     * @param pid The pid of the process to read the thread of
     * @param tid The tid of the thread to read
     * @return The thread data for the thread tid in the process with pid
     */
    Thread readThread(Pid_t pid, Pid_t tid, bool readIo);
}

#endif //WEASEL_PROCREADER_HPP
