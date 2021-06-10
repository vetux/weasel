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

#include "os/thread.hpp"
#include "os/process.hpp"
#include "os/systemstatus.hpp"

namespace ProcReader {
    bool isPID(const std::string &name);

    Thread readThread(Process &process, Pid_t tid);

    Process readProcess(Pid_t pid);

    SystemStatus readSystem();
}

#endif //WEASEL_PROCREADER_HPP
