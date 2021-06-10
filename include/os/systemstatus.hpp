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

#include "os/types.hpp"

struct SystemStatus {
    struct Core {

    };

    // /proc/stat
    Core cpu;
    std::vector<Core> cores;

    // /proc/meminfo
    Mem_t total;
    Mem_t free;
    Mem_t available;
};

#endif //WEASEL_SYSTEMSTATUS_HPP
