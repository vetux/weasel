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

#include "weasel/types.hpp"

/**
 * The thread data structure contains the thread specific data and exposes methods for invoking signals or changing
 * the scheduling policy of the thread.
 */
struct Thread {
    enum SchedulingPolicy {
        OTHER,
        BATCH,
        IDLE,
        FIFO,
        RR,
        DEADLINE
    };

    enum Signal {
        SIGNAL_SIGHUP,
        SIGNAL_SIGINT,
        SIGNAL_SIGQUIT,
        SIGNAL_SIGILL,
        SIGNAL_SIGABRT,
        SIGNAL_SIGFPE,
        SIGNAL_SIGKILL,
        SIGNAL_SIGSEGV,
        SIGNAL_SIGPIPE,
        SIGNAL_SIGALRM,
        SIGNAL_SIGTERM,
        SIGNAL_SIGUSR1,
        SIGNAL_SIGUSR2,
        SIGNAL_SIGCHLD,
        SIGNAL_SIGCONT,
        SIGNAL_SIGSTOP,
        SIGNAL_SIGTSTP,
        SIGNAL_SIGTTIN,
        SIGNAL_SIGTTOU
    };

    Pid_t pid{}; // The pid of the process which this thread belongs to, (main thread tid == pid)
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
    SchedulingPolicy policy{};
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

    // /proc/[pid]/task/[tid]/io
    unsigned long rchar{};
    unsigned long wchar{};
    unsigned long syscr{};
    unsigned long syscw{};
    unsigned long read_bytes{};
    unsigned long write_bytes{};
    unsigned long cancelled_write_bytes{};

    // The rates for /proc/[pid]/task/[tid]/io values calculated at runtime by the snapshot generator
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

    // The total load on the scheduler of this thread.
    // Relative to the power of one core,
    // eg. if there are 4 cores and the thread is using full cycles we would see a usage value of 1
    // This is the approach htop/top/ps seem to take, to get the value relative to all cores the usage value can be divided by the number of cores.
    float usage{0};

    bool isMainThread() const { return tid == pid; }

    /**
     * Send the signal to the thread.
     *
     * @param thread The thread to send the signal to
     * @param signal The signal to send
     */
    void sendSignal(Signal signal) const;

    /**
     * Set the thread nice value.
     *
     * @param thread The thread to set the priority of
     * @param priority The priority to set
     */
    void setPriority(int priority) const;

    /**
     * Set the priority and scheduling policy of the thread.
     *
     * @param thread The thread to set the policy of.
     * @param policy The policy to set.
     * @param runtime If policy is DEADLINE the runtime value in nanoseconds.
     * @param deadline If policy is DEADLINE the deadline value in nanoseconds.
     * @param period If policy is DEADLINE the period value in nanoseconds.
     */
    void setSchedulerAttributes(int priority,
                                SchedulingPolicy policy,
                                uint64_t runtime = 0,
                                uint64_t deadline = 0,
                                uint64_t period = 0) const;

    /**
     * Send the signal to the thread.
     *
     * @param thread The thread to send the signal to
     * @param signal The signal to send
     */
    static void sendSignal(Pid_t tid, Signal signal);

    /**
     * Set the thread nice value.
     *
     * @param thread The thread to set the priority of
     * @param priority The priority to set
     */
    static void setPriority(Pid_t tid, int priority);

    /**
     * Set the scheduling policy of the thread.
     *
     * @param thread The thread to set the policy of.
     * @param policy The policy to set.
     * @param runtime If policy is DEADLINE the runtime value in nanoseconds.
     * @param deadline If policy is DEADLINE the deadline value in nanoseconds.
     * @param period If policy is DEADLINE the period value in nanoseconds.
     */
    static void setSchedulerAttributes(Pid_t tid,
                                       int priority,
                                       SchedulingPolicy policy,
                                       uint64_t runtime = 0,
                                       uint64_t deadline = 0,
                                       uint64_t period = 0);
};

#endif //WEASEL_THREAD_HPP
