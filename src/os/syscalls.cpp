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

#include "syscalls.hpp"

#include <sys/syscall.h>

long tkill(pid_t TID, int signal) {
    return syscall(SYS_tkill, TID, signal);
}

int sched_setattr(pid_t pid, struct sched_attr *attr, unsigned int flags) {
    return static_cast<int>(syscall(SYS_sched_setattr, pid, attr, flags));
}

int sched_getattr(pid_t pid, struct sched_attr *attr, unsigned int size, unsigned int flags) {
    return static_cast<int>(syscall(SYS_sched_getattr, pid, attr, size, flags));
}
