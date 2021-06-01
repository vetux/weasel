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
     * This method polls the operating system for active processes and returns them.
     *
     * The scheduler is responsible for deleting the process pointers.
     *
     * The process and thread interfaces are read only interfaces, the only way for code to change something
     * in the process tree is through signals passing through the scheduler abstraction,
     * or other scheduler member methods (eg process priority).
     *
     * @return The map of currently active processes with the PID as key.
     */
    virtual const std::map<int, Process *> &getProcesses() = 0;

    /**
     * Send the signal to a process.
     *
     * @param process The process to send the signal to
     * @param signal The signal to send
     */
    virtual void signal(Process *process, Signal signal) = 0;

    /**
     * Send the signal to a thread.
     *
     * @param thread The thread to send the signal to
     * @param signal The signal to send
     */
    virtual void signal(Thread *thread, Signal signal) = 0;
};

#endif //WEASEL_SCHEDULER_HPP
