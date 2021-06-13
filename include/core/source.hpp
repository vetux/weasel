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

#ifndef WEASEL_SCANNER_HPP
#define WEASEL_SCANNER_HPP

#include <map>

#include "core/systemstatus.hpp"
#include "core/process.hpp"

class Source {
public:
    /**
    * This method updates the data.
    */
    void refresh();

    /**
     * @return The system status data
     */
    const SystemStatus &getSystemStatus();

    /**
     * The process and thread interfaces are plain data objects.
     *
     * To modify the process tree methods on the scheduler interface have to be used.
     *
     * @return The map of currently active processes with the pid as key.
     */
    const std::map<Pid_t, Process> &getProcesses();

private:
    SystemStatus system;
    std::map<Pid_t, Process> processes;
};

#endif //WEASEL_SCANNER_HPP
