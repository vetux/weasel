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

#include "weasel/thread.hpp"
#include "weasel/types.hpp"
#include "weasel/socket.hpp"

/**
 * The process data structure contains the data which is shared between threads and
 * 1 or more thread states.
 */
struct Process {
    Pid_t pid{}; // The process id, equal to threads[0].tid
    Uid_t uid{}; // User ID retrieved via stat()
    std::string userName; //The user name for the uid

    // /proc/[pid]/cmdline
    std::string commandLine{};

    // /proc/[pid]/environ
    std::vector<std::string> environ{};

    // /proc/[pid]/root
    std::string rootDirectory{};

    // /proc/[pid]/fd/
    std::vector<Socket> sockets;
    std::vector<std::string> files;

    // /proc/[pid]/exe
    std::string executablePath{};

    // /proc/[pid]/cwd
    std::string cwd{};

    // /proc/[pid]/stat - The status of the process.
    // The task stat data appears to be unique per thread so the main thread stat does not have to be "Deaccumulated"
    // /proc/[pid]/stat appears to NOT be the same as /proc/[pid]/task/[pid]/stat
    // we assume the process stat includes all threads.
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
    Thread::SchedulingPolicy policy{};
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

    // /proc/[pid]/statm    - The memory status for this process, as threads share the address space this is per process.
    unsigned int size{};
    unsigned int resident{};
    unsigned int shared{};
    unsigned int text{};
    unsigned int lib{};
    unsigned int data{};
    unsigned int dt{};

    // /proc/[pid]/io   -   The total io of all threads in the process, the main thread contains the subtracted values.
    unsigned long rchar{};
    unsigned long wchar{};
    unsigned long syscr{};
    unsigned long syscw{};
    unsigned long read_bytes{};
    unsigned long write_bytes{};
    unsigned long cancelled_write_bytes{};

    // The rates for /proc/[pid]/io values calculated at runtime by the snapshot generator
    unsigned long rate_rchar{0};
    unsigned long rate_wchar{0};
    unsigned long rate_syscr{0};
    unsigned long rate_syscw{0};
    unsigned long rate_read_bytes{0};
    unsigned long rate_write_bytes{0};

    //TODO: Network IO statistics (libpcap)
    unsigned long network_read_bytes{0};
    unsigned long network_write_bytes{0};
    unsigned long network_read_rate{0};
    unsigned long network_write_rate{0};

    // The total load on the scheduler that all the threads in this process take.
    // Relative to the power of one core,
    // eg. if there are 4 cores and the process has 2 threads using full cycles we would see a usage value of 2
    // This is the approach htop/top/ps seem to take, to get the value relative to all cores the usage value can be divided by the number of cores.
    float usage{0};

    std::map<Pid_t, Thread> threads{}; // The threads, threads.at(pid) is the main thread.

    const Thread &mainThread() const { return threads.at(pid); }

    Thread &mainThread() { return threads.at(pid); }
};

#endif //WEASEL_PROCESS_HPP
