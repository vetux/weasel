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

#ifndef WEASEL_PROCESSREADFLAGS_HPP
#define WEASEL_PROCESSREADFLAGS_HPP

enum ProcessReadFlags {
    READ_NONE = 0,
    READ_PROCESS_THREADS = 1, // Read all the threads of the process instead of just the main thread
    READ_PROCESS_IO = 2, // Read the process io statistics
    READ_PROCESS_FD = 4, // Read the process file descriptors (Open files and sockets)
    READ_THREAD_IO = 8 // Read thread io for threads in the process
};

inline ProcessReadFlags operator|(ProcessReadFlags a,
                                  ProcessReadFlags b) {
    return static_cast<ProcessReadFlags>(static_cast<int>(a) | static_cast<int>(b));
}

#endif //WEASEL_PROCESSREADFLAGS_HPP
