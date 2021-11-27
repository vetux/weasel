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

#ifndef WEASEL_CPUSTATE_HPP
#define WEASEL_CPUSTATE_HPP

#include <vector>

struct CpuState {
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

        float load{0}; //The load of the core calculated by the snapshot generator in the range 0 - 1
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
};

#endif //WEASEL_CPUSTATE_HPP
