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

#include "system/thread.hpp"
#include "system/types.hpp"
#include "system/signal.hpp"

struct Process {
    Pid_t pid{}; // The process id, equal to threads[0].tid
    Uid_t uid{}; // User ID retrieved via stat()

    // /proc/[pid]/cmdline
    std::string commandLine{};

    // /proc/[pid]/environ
    std::vector<std::string> environ{};

    // /proc/[pid]/io
    unsigned long rchar{};
    unsigned long wchar{};
    unsigned long syscr{};
    unsigned long syscw{};
    unsigned long read_bytes{};
    unsigned long write_bytes{};
    unsigned long cancelled_write_bytes{};

    // /proc/[pid]/root
    std::string rootDirectory{};

    // /proc/[pid]/fd/
    std::vector<std::string> openFiles{};

    // /proc/[pid]/exe
    std::string executablePath{};

    std::vector<Thread> threads{}; // The threads, the thread at index 0 is always the process main thread

    const Thread &mainThread() const;

    Thread &mainThread();

    /**
     * Send the signal to the process.
     *
     * @param signal The signal to send
     */
    void sendSignal(Signal signal) const;

    /**
     * Set the process nice value.
     *
     * The priority value is in the range -20 to 19, with -20 being the highest priority and 19 being the lowest priority.
     *
     * @param priority The priority to set.
     */
    void setPriority(int priority) const;
};

#endif //WEASEL_PROCESS_HPP
