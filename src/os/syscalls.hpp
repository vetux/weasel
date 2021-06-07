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

#ifndef WEASEL_SYSCALLS_HPP
#define WEASEL_SYSCALLS_HPP

/**
 * This header defines linux syscalls which are not wrapped by glibc.
 */

#include <cstdint>

#include <unistd.h>
#include <sched.h>

#include <sys/types.h>

long tkill(pid_t TID, int signal);

//https://man7.org/linux/man-pages/man2/sched_getattr.2.html
struct sched_attr {
    uint32_t size;              /* Size of this structure */
    uint32_t sched_policy;      /* Policy (SCHED_*) */
    uint64_t sched_flags;       /* Flags */
    int32_t sched_nice;        /* Nice value (SCHED_OTHER, SCHED_BATCH) */

    uint32_t sched_priority;    /* Static priority (SCHED_FIFO, SCHED_RR) */

    /* Remaining fields are for SCHED_DEADLINE */
    uint64_t sched_runtime;
    uint64_t sched_deadline;
    uint64_t sched_period;
};

int sched_setattr(pid_t pid, struct sched_attr *attr, unsigned int flags);

int sched_getattr(pid_t pid, struct sched_attr *attr, unsigned int size, unsigned int flags);

#endif //WEASEL_SYSCALLS_HPP
