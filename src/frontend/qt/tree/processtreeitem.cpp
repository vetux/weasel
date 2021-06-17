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

#include "frontend/qt/tree/processtreeitem.hpp"

#include "system/user.hpp"

#define BYTES_MEGABYTE 1000000

#define BYTES_GIGABYTE 1000000000

static float getCpuPercentage(const SystemStatus &system,
                              const SystemStatus &prevSystem,
                              const Thread &thread,
                              const Thread &prev) {
    //https://github.com/scaidermern/top-processes/blob/2a2b1a84282b0418f51fd145133175dbff6e155a/top_proc.c#L158
    auto total_cpu = system.cpu.user + system.cpu.nice + system.cpu.system + system.cpu.idle
                     + system.cpu.iowait + system.cpu.irq + system.cpu.softirq + system.cpu.steal;
    auto prev_total_cpu = prevSystem.cpu.user + prevSystem.cpu.nice + prevSystem.cpu.system + prevSystem.cpu.idle
                          + prevSystem.cpu.iowait + prevSystem.cpu.irq + prevSystem.cpu.softirq + prevSystem.cpu.steal;
    auto total = thread.utime + thread.stime;
    auto prevTotal = prev.utime + prev.stime;
    auto cpuTime = total - prevTotal;
    auto percentage = (cpuTime / (float) (total_cpu - prev_total_cpu) * 100.0 * system.cores.size());
    return percentage;
}

static std::string getMemoryString(unsigned long bytes) {
    if (bytes >= BYTES_GIGABYTE) {
        return std::to_string(bytes / BYTES_GIGABYTE) + "G";
    } else if (bytes >= BYTES_MEGABYTE) {
        return std::to_string(bytes / BYTES_MEGABYTE) + "M";
    } else {
        return std::to_string(bytes);
    }
}

static void replace(std::string &str, const std::string &v, const std::string &r) {
    size_t pos = str.find(v);
    while (pos != std::string::npos) {
        str.replace(pos, v.size(), r);
        pos = str.find(v);
    }
}

ProcessTreeItem::ProcessTreeItem() = default;

ProcessTreeItem::ProcessTreeItem(const SystemStatus &status,
                                 const SystemStatus &prevStatus,
                                 const Process &process,
                                 const Process &prevProc)
        : process(process),
          rowItems(),
          QStandardItem() {
    rowItems.append(this);

    for (int i = 0; i < 7; i++)
        rowItems.append(new QStandardItem());

    setProcess(status, prevStatus, process, prevProc);
}

ProcessTreeItem::~ProcessTreeItem() = default;

void ProcessTreeItem::setProcess(const SystemStatus &s,
                                 const SystemStatus &prevStatus,
                                 const Process &p,
                                 const Process &prevProc) {
    process = p;
    setName(QString("%0").arg(process.mainThread().comm.c_str()));
    setPid(QString("%0").arg(process.mainThread().pid));
    setUser(QString("%0").arg(User::getUserName(process.uid).c_str()));
    setCpu(QString("%0").arg(getCpuPercentage(s, prevStatus, p.mainThread(), prevProc.mainThread()), 0, 'f', 2));
    setVirtual(QString("%0").arg(getMemoryString(p.mainThread().vsize).c_str()));
    setResident(QString("%0").arg(getMemoryString(p.mainThread().resident * SystemStatus::getPageSize()).c_str()));
    setShared(QString("%0").arg(getMemoryString(p.mainThread().shared * SystemStatus::getPageSize()).c_str()));

    std::string c = p.commandLine;
    replace(c, std::string("\0", 1), " ");
    setCommand(QString("%0").arg(c.c_str()));
}

const Process &ProcessTreeItem::getProcess() {
    return process;
}

const QList<QStandardItem *> &ProcessTreeItem::getRowItems() {
    return rowItems;
}

void ProcessTreeItem::setName(const QString &name) {
    rowItems.at(0)->setText(name);
}

void ProcessTreeItem::setPid(const QString &pid) {
    rowItems.at(1)->setText(pid);
}

void ProcessTreeItem::setUser(const QString &user) {
    rowItems.at(2)->setText(user);
}

void ProcessTreeItem::setCpu(const QString &cpu) {
    rowItems.at(3)->setText(cpu);
}

void ProcessTreeItem::setVirtual(const QString &mem) {
    rowItems.at(4)->setText(mem);
}

void ProcessTreeItem::setResident(const QString &mem) {
    rowItems.at(5)->setText(mem);
}

void ProcessTreeItem::setShared(const QString &mem) {
    rowItems.at(6)->setText(mem);
}

void ProcessTreeItem::setCommand(const QString &command) {
    rowItems.at(7)->setText(command);
}
