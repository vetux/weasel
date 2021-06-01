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

#include "sched/thread.hpp"

class Process {
public:
    virtual ~Process() = default;

    /**
     * Stop the process
     */
    virtual void stop() = 0;

    /**
     * Resume the stopped process
     */
    virtual void resume() = 0;

    /**
     * Kill the process and all its child processes / threads
     */
    virtual void kill() = 0;

    /**
     * @return The name of the process
     */
    virtual std::string getName() = 0;

    /**
     * @return The process id
     */
    virtual int getPID() = 0;

    /**
     * @return The cpu usage between 0 - 1
     */
    virtual float getCPU() = 0;

    /**
     * @return The number of bytes of virtual memory.
     */
    virtual long getVirtualMemorySize() = 0;

    /**
     * @return The number of bytes of reserved memory.
     */
    virtual long getReservedMemorySize() = 0;

    /**
     * @return The number of bytes of shared memory.
     */
    virtual long getSharedMemorySize() = 0;

    /**
     * @return The PID of the parent process if it has one, otherwise -1
     */
    virtual int getParent() = 0;

    /**
     * @return The list of threads associated with this process, thread 0 is always the main (Process) thread.
     */
    virtual std::vector<Thread *> getThreads() = 0;
};

#endif //WEASEL_PROCESS_HPP
