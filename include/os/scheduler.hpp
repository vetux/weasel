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

#include <map>

#include "os/process.hpp"
#include "os/signal.hpp"
#include "os/memory.hpp"

class Scheduler {
public:
    /**
     * This method polls the operating system for active processes and returns a reference to them.
     *
     * The process and thread interfaces are plain data objects.
     *
     * To modify the process tree methods on the scheduler interface have to be used.
     *
     * @return The map of currently active processes with the PID as key.
     */
    const std::map<Pid_t, Process> &getProcesses();

    /**
     * This method polls the operating system for memory information and returns a reference to it.
     *
     * @return The memory information
     */
    const Memory &getMemory();

    /**
     * Send the signal to the process.
     *
     * @param process The process to send the signal to
     * @param signal The signal to send
     */
    void signal(const Process &process, Signal signal);

    /**
     * Send the signal to the thread.
     *
     * @param thread The thread to send the signal to
     * @param signal The signal to send
     */
    void signal(const Thread &thread, Signal signal);

    /**
     * Set the process nice value.
     *
     * The priority value is in the range -20 to 19, with -20 being the highest priority and 19 being the lowest priority.
     *
     * @param process The process to set the priority of.
     * @param priority The priority to set.
     */
    void setPriority(const Process &process, int priority);

    /**
     * Set the thread nice value.
     *
     * @param thread The thread to set the priority of
     * @param priority The priority to set
     */
    void setPriority(const Thread &thread, int priority);

    /**
     * Set the scheduling policy of the thread.
     *
     * @param thread The thread to set the policy of.
     * @param policy The policy to set.
     * @param runtime If policy is DEADLINE the runtime value in nanoseconds.
     * @param deadline If policy is DEADLINE the deadline value in nanoseconds.
     * @param period If policy is DEADLINE the period value in nanoseconds.
     */
    void setPolicy(const Thread &thread,
                   SchedulingPolicy policy,
                   uint64_t runtime = 0,
                   uint64_t deadline = 0,
                   uint64_t period = 0);

private:
    std::map<Pid_t, Process> processes;
    Memory memory;
};

#endif //WEASEL_SCHEDULER_HPP
