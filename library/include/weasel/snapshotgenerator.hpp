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

#ifndef WEASEL_SNAPSHOTGENERATOR_HPP
#define WEASEL_SNAPSHOTGENERATOR_HPP

#include <chrono>
#include <set>

#include "weasel/snapshot.hpp"
#include "weasel/processreadflags.hpp"

class SnapshotGenerator {
public:
    const Snapshot &next();

    /**
     * Set the read flags for the process
     *
     * @param pid
     */
    void setProcessReadFlags(Pid_t pid, ProcessReadFlags flag) {
        processReadFlags[pid] = flag;
    }

    void clearProcessReadFlags(Pid_t pid) {
        processReadFlags.erase(pid);
    }

    void setDefaultReadFlags(ProcessReadFlags flags) {
        defaultReadFlags = flags;
    }

private:
    std::chrono::high_resolution_clock::time_point lastCreate;
    Snapshot currentSnapshot;

    std::map<Pid_t, ProcessReadFlags> processReadFlags;
    ProcessReadFlags defaultReadFlags = READ_NONE;
};

#endif //WEASEL_SNAPSHOTGENERATOR_HPP
