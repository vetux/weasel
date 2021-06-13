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

#ifndef WEASEL_THREAD_HPP
#define WEASEL_THREAD_HPP

#include <string>

#include "core/schedulingpolicy.hpp"
#include "core/types.hpp"

struct Thread {
    Pid_t pid{}; // The pid of the process which this thread belongs to, (main thread tid == pid)
    Uid_t uid{}; // User ID retrieved via stat()
    Pid_t tid{}; // The thread id

    //Reference: https://man7.org/linux/man-pages/man5/proc.5.html

    // /proc/[pid]/task/[tid]/stat
    std::string comm{};
    char state{};
    Pid_t ppid{};
    Pid_t pgrp{};
    Pid_t session{};
    int tty_nr{};
    Pid_t tpgid{};
    unsigned int flags{};
    unsigned long minflt{};
    unsigned long cminflt{};
    unsigned long majflt{};
    unsigned long cmajflt{};
    unsigned long utime{};
    unsigned long stime{};
    long cutime{};
    long cstime{};
    long priority{};
    long nice{};
    long num_threads{};
    long itrealvalue{};
    unsigned long long starttime{};
    unsigned long vsize{};
    long rss{};
    unsigned long rsslim{};
    unsigned long startcode{};
    unsigned long endcode{};
    unsigned long startstack{};
    unsigned long kstkesp{};
    unsigned long kstkeip{};
    unsigned long signal{};
    unsigned long blocked{};
    unsigned long sigignore{};
    unsigned long sigcatch{};
    unsigned long wchan{};
    unsigned long nswap{};
    unsigned long cnswap{};
    int exit_signal{};
    int processor{};
    unsigned int rt_priority{};
    unsigned int policy{};
    unsigned long long delayacct_blkio_ticks{};
    unsigned long guest_time{};
    long cguest_time{};
    unsigned long start_data{};
    unsigned long end_data{};
    unsigned long start_brk{};
    unsigned long arg_start{};
    unsigned long arg_end{};
    unsigned long env_start{};
    unsigned long env_end{};
    unsigned long exit_code{};

    // /proc/[pid]/task/[tid]/statm
    unsigned int size{};
    unsigned int resident{};
    unsigned int shared{};
    unsigned int text{};
    unsigned int lib{};
    unsigned int data{};
    unsigned int dt{};

    // /proc/[pid]/task/[tid]/cwd
    std::string cwd{};
};

#endif //WEASEL_THREAD_HPP
