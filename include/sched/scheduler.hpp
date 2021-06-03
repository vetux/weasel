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

#ifndef WEASEL_SCHEDULER_HPP
#define WEASEL_SCHEDULER_HPP

#include "sched/process.hpp"
#include "sched/platform.hpp"
#include "sched/signal.hpp"
#include "sched/memory.hpp"

class Scheduler {
public:
    /**
     * This method creates the scheduler instance suitable for the platform targeted when compiling the application.
     *
     * @return A new scheduler instance
     */
    static Scheduler *createScheduler();

    /**
     * @return The platform targeted when compiling the application.
     */
    static Platform getPlatform();

    virtual ~Scheduler() = default;

    /**
     * This method polls the operating system for active processes and returns a reference to them.
     *
     * The process and thread interfaces are plain data objects.
     *
     * To modify the process tree methods on the scheduler interface have to be used.
     *
     * @return The map of currently active processes with the PID as key.
     */
    virtual const std::map<int, Process> &getProcesses() = 0;

    /**
     * This method polls the operating system for memory information and returns a reference to it.
     *
     * @return The memory information
     */
    virtual const Memory &getMemory() = 0;

    /**
     * Send the signal to the process
     *
     * @param process The process to send the signal to
     * @param signal The signal to send
     */
    virtual void signal(const Process &process, Signal signal) = 0;

    /**
     * Send the signal to the thread
     *
     * @param thread The thread to send the signal to
     * @param signal The signal to send
     */
    virtual void signal(const Thread &thread, Signal signal) = 0;

    /**
     * Set the process priority
     *
     * @param process The process to set the priority of
     * @param priority The priority to set
     */
    virtual void setPriority(const Process &process, int priority) = 0;

    /**
     * Set the thread priority
     *
     * @param thread The thread to set the priority of
     * @param priority The priority to set
     */
    virtual void setPriority(const Thread &thread, int priority) = 0;
};

#endif //WEASEL_SCHEDULER_HPP
