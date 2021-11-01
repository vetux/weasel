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
#include <map>

#include "system/thread.hpp"
#include "system/types.hpp"
#include "system/socket.hpp"

struct Process {
    Pid_t pid{}; // The process id, equal to threads[0].tid
    Uid_t uid{}; // User ID retrieved via stat()
    std::string userName; //The user name for the uid

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
    std::vector<Socket> sockets;
    std::vector<std::string> files;

    // /proc/[pid]/exe
    std::string executablePath{};

    std::vector<Thread> threads{}; // The threads, the thread at index 0 is always the process main thread

    const Thread &mainThread() const { return threads.at(0); }

    Thread &mainThread() { return threads.at(0); }
};

#endif //WEASEL_PROCESS_HPP
