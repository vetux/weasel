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

#ifndef WEASEL_PROCPATH_HPP
#define WEASEL_PROCPATH_HPP

#include <string>

namespace ProcPath {
    std::string getProcDirectory();

    std::string getProcessDirectory(int PID);

    std::string getThreadDirectory(int PID, int TID);

    std::string getProcessTasksDirectory(int PID);

    std::string getProcessStatusFile(int PID);

    std::string getThreadStatusFile(int PID, int TID);

    std::string getCommandLineFile(int PID);

    std::string getMemoryInfoFile();
}

#endif //WEASEL_PROCPATH_HPP
