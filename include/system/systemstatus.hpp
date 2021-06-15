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

#ifndef WEASEL_SYSTEMSTATUS_HPP
#define WEASEL_SYSTEMSTATUS_HPP

#include <vector>

#include "system/types.hpp"

struct SystemStatus {
    struct Core {
        unsigned long user;
        unsigned long nice;
        unsigned long system;
        unsigned long idle;
        unsigned long iowait;
        unsigned long irq;
        unsigned long softirq;
        unsigned long steal;
        unsigned long guest;
        unsigned long guest_nice;
    };

    // /proc/stat
    Core cpu;
    std::vector<Core> cores;
    long pageIn;
    long pageOut;
    long swapIn;
    long swapOut;
    long interrupts;
    long contextSwitches;
    unsigned long btime;
    long processes;
    long processesRunning;
    long processesBlocked;
    long softIRQ;

    // /proc/meminfo
    Mem_t total;
    Mem_t free;
    Mem_t available;
    Mem_t buffers;
    Mem_t cached;
    Mem_t swapCached;
    Mem_t active;
    Mem_t inactive;
    Mem_t anonActive;
    Mem_t anonInactive;
    Mem_t fileActive;
    Mem_t fileInactive;
    Mem_t unevictable;
    Mem_t locked;
    Mem_t highTotal;
    Mem_t highFree;
    Mem_t lowTotal;
    Mem_t lowFree;
    Mem_t mmapCopy;
    Mem_t swapTotal;
    Mem_t swapFree;
    Mem_t dirty;
    Mem_t writeBack;
    Mem_t anonPages;
    Mem_t mapped;
    Mem_t shmem;
    Mem_t kreclaimable;
    Mem_t slab;
    Mem_t sreclaimable;
    Mem_t sunreclaim;
    Mem_t kernelStack;
    Mem_t pageTables;
    Mem_t quickLists;
    Mem_t nfsUnstable;
    Mem_t bounce;
    Mem_t writebackTmp;
    Mem_t commitLimit;
    Mem_t commitedAs;
    Mem_t vmAllocTotal;
    Mem_t vmAllocUsed;
    Mem_t vmAllocChunk;
    Mem_t hardwareCorrupted;
    Mem_t lazyFree;
    Mem_t anonHugePages;
    Mem_t shmemHugePages;
    Mem_t shmemPmdMapped;
    Mem_t cmaTotal;
    Mem_t cmaFree;
    Mem_t hugePagesTotal;
    Mem_t hugePagesFree;
    Mem_t hugePagesRsvd;
    Mem_t hugePagesSurp;
    Mem_t hugePageSize;
    Mem_t directMap4K;
    Mem_t directMap4M;
    Mem_t directMap2M;
    Mem_t directMap1G;
};

#endif //WEASEL_SYSTEMSTATUS_HPP
