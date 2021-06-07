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

#include "os/procpath.hpp"

#define LINUX_DIR_PROC std::string("/proc")

#define LINUX_DIR_PROCESS(PID) LINUX_DIR_PROC + "/" + PID
#define LINUX_DIR_PROCESS_TASKS(PID) LINUX_DIR_PROCESS(PID) + "/task"

#define LINUX_DIR_THREAD(PID, TID) LINUX_DIR_PROCESS(PID) + "/task/" + TID

#define LINUX_STATUS_PROCESS(PID) LINUX_DIR_PROCESS(PID) + "/status"
#define LINUX_STATUS_THREAD(PID, TID) LINUX_DIR_THREAD(PID, TID) + "/status"

#define LINUX_COMMAND_LINE(PID) LINUX_DIR_PROCESS(PID) + "/cmdline"

#define LINUX_MEMINFO LINUX_DIR_PROC + "/meminfo"

namespace ProcPath {
    std::string getProcDirectory() {
        return LINUX_DIR_PROC;
    }

    std::string getProcessDirectory(int PID) {
        return LINUX_DIR_PROCESS(std::to_string(PID));
    }

    std::string getThreadDirectory(int PID, int TID) {
        return LINUX_DIR_THREAD(std::to_string(PID), std::to_string(TID));
    }

    std::string getProcessTasksDirectory(int PID) {
        return LINUX_DIR_PROCESS_TASKS(std::to_string(PID));
    }

    std::string getProcessStatusFile(int PID) {
        return LINUX_STATUS_PROCESS(std::to_string(PID));
    }

    std::string getThreadStatusFile(int PID, int TID) {
        return LINUX_STATUS_THREAD(std::to_string(PID), std::to_string(TID));
    }

    std::string getCommandLineFile(int PID) {
        return LINUX_COMMAND_LINE(std::to_string(PID));
    }

    std::string getMemoryInfoFile() {
        return LINUX_MEMINFO;
    }
}

