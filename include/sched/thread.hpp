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

#include "sched/executionstate.hpp"

class Thread {
public:
    virtual ~Thread() = default;

    /**
     * Stop the thread
     */
    virtual void stop() = 0;

    /**
     * Resume the stopped thread
     */
    virtual void resume() = 0;

    /**
     * Kill the thread
     */
    virtual void kill() = 0;

    /**
     * @return The state of the thread
     */
    virtual ExecutionState getState() = 0;

    /**
     * @return The thread name
     */
    virtual std::string getName() = 0;

    /**
     * @return The thread id
     */
    virtual int getTID() = 0;

    /**
     * @return The cpu usage of the thread between 0 - 1
     */
    virtual float getCPU() = 0;

    /**
     * @return The number of bytes of virtual memory used by the thread.
     */
    virtual long getVirtualMemorySize() = 0;

    /**
     * @return The number of bytes of reserved memory used by the thread.
     */
    virtual long getReservedMemorySize() = 0;

    /**
     * @return The number of bytes of shared memory used by the thread.
     */
    virtual long getSharedMemorySize() = 0;

    /**
     * @return The PID of the containing process
     */
    virtual int getProcess() = 0;
};

#endif //WEASEL_THREAD_HPP
