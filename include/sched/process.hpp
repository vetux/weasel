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
#include "sched/signal.hpp"

class Process {
public:
    virtual ~Process() = default;

    /**
     * Send the signal to the process.
     *
     * @param signal The signal to send
     */
    virtual void signal(Signal signal) = 0;

    /**
     * Set the process priority value between +19 (low priority) to -20 (high priority).
     *
     * @param priority The new priority value.
     */
    virtual void setPriority(int priority) = 0;

    /**
     * Get the process priority value.
     *
     * @return The process priority value
     */
    virtual int getPriority() = 0;

    /**
     * @return The user name of the process owner.
     */
    virtual const std::string &getUser() = 0;

    /**
     * @return The name of the process
     */
    virtual std::string getName() = 0;

    /**
     * @return The description of the process
     */
    virtual std::string getDescription() = 0;

    /**
     * @return The absolute path of the process executable image
     */
    virtual std::string getImagePath() = 0;

    /**
     * @return true if the process is paused
     */
    virtual bool isPaused() = 0;

    /**
     * @return The process id
     */
    virtual int getPID() = 0;

    /**
     * @return The cpu usage of the process.
     */
    virtual float getCPU() = 0;

    /**
     * @return The number of bytes of virtual memory used by the process.
     */
    virtual long getVirtualMemorySize() = 0;

    /**
     * @return The number of bytes of reserved memory used by the process.
     */
    virtual long getReservedMemorySize() = 0;

    /**
     * @return The number of bytes of shared memory used by the process.
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
