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

#include "os/types.hpp"

#define PATH_PROC std::string("/proc")
#define PATH_TASK std::string("/task")
#define PATH_STATUS std::string("/status")
#define PATH_STAT std::string("/stat")
#define PATH_STATM std::string("/statm")
#define PATH_ENVIRON std::string("/environ")
#define PATH_CWD std::string("/cwd")
#define PATH_EXE std::string("/exe")
#define PATH_FD std::string("/fd")
#define PATH_IO std::string("/io")
#define PATH_ROOT std::string("/root")
#define PATH_CMDLINE std::string("/cmdline")
#define PATH_MEMINFO std::string("/meminfo")

inline std::string operator+(const std::string &l, Pid_t r) {
    return l + std::to_string(r);
}

namespace ProcPath {
    inline std::string getProcDirectory() {
        return PATH_PROC;
    }

    inline std::string getProcessDirectory(Pid_t pid) {
        return PATH_PROC + "/" + pid;
    }

    inline std::string getProcessTasksDirectory(Pid_t pid) {
        return getProcessDirectory(pid) + PATH_TASK;
    }

    inline std::string getProcessStatusFile(Pid_t pid) {
        return getProcessDirectory(pid) + PATH_STATUS;
    }

    inline std::string getProcessCommandLineFile(Pid_t pid) {
        return getProcessDirectory(pid) + PATH_CMDLINE;
    }

    inline std::string getProcessEnvironFile(Pid_t pid) {
        return getProcessDirectory(pid) + PATH_ENVIRON;
    }

    inline std::string getProcessIoFile(Pid_t pid) {
        return getProcessDirectory(pid) + PATH_IO;
    }

    inline std::string getProcessRootDirectory(Pid_t pid) {
        return getProcessDirectory(pid) + PATH_ROOT;
    }

    inline std::string getProcessFdDirectory(Pid_t pid) {
        return getProcessDirectory(pid) + PATH_FD;
    }

    inline std::string getProcessExeDirectory(Pid_t pid) {
        return getProcessDirectory(pid) + PATH_EXE;
    }

    inline std::string getThreadDirectory(Pid_t pid, Pid_t tid) {
        return getProcessDirectory(pid) + PATH_TASK + "/" + tid;
    }

    inline std::string getThreadStatusFile(Pid_t pid, Pid_t tid) {
        return getThreadDirectory(pid, tid) + PATH_STATUS;
    }

    inline std::string getThreadStatFile(Pid_t pid, Pid_t tid) {
        return getThreadDirectory(pid, tid) + PATH_STAT;
    }

    inline std::string getThreadStatMFile(Pid_t pid, Pid_t tid) {
        return getThreadDirectory(pid, tid) + PATH_STATM;
    }

    inline std::string getThreadCwdFile(Pid_t pid, Pid_t tid) {
        return getThreadDirectory(pid, tid) + PATH_CWD;
    }

    inline std::string getThreadExeFile(Pid_t pid, Pid_t tid) {
        return getThreadDirectory(pid, tid) + PATH_EXE;
    }

    inline std::string getThreadFdDirectory(Pid_t pid, Pid_t tid) {
        return getThreadDirectory(pid, tid) + PATH_FD;
    }

    inline std::string getThreadIoDirectory(Pid_t pid, Pid_t tid) {
        return getThreadDirectory(pid, tid) + PATH_IO;
    }

    inline std::string getThreadRootDirectory(Pid_t pid, Pid_t tid) {
        return getThreadDirectory(pid, tid) + PATH_ROOT;
    }

    inline std::string getMemoryInfoFile() {
        return PATH_PROC + PATH_MEMINFO;
    }
};

#endif //WEASEL_PROCPATH_HPP
