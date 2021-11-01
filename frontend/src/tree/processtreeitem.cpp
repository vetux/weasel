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

#include "tree/processtreeitem.hpp"

#include "strutil.hpp"
#include "byteformatting.hpp"

static std::string getMemoryString(unsigned long bytes) {
    if (bytes >= BYTES_GIGABYTE) {
        return std::to_string(bytes / BYTES_GIGABYTE) + "G";
    } else if (bytes >= BYTES_MEGABYTE) {
        return std::to_string(bytes / BYTES_MEGABYTE) + "M";
    } else {
        return std::to_string(bytes);
    }
}

ProcessTreeItem::ProcessTreeItem(Pid_t pid)
        : QStandardItem(), pid(pid){
    rowItems.append(this);

    for (int i = 0; i < 7; i++)
        rowItems.append(new QStandardItem());
}

ProcessTreeItem::~ProcessTreeItem() = default;

Pid_t ProcessTreeItem::getPid() const {
    return pid;
}

const QList<QStandardItem *> &ProcessTreeItem::getRowItems() {
    return rowItems;
}

void ProcessTreeItem::setName(const QString &name) {
    rowItems.at(0)->setText(name);
}

void ProcessTreeItem::setPid(const QString &str) {
    rowItems.at(1)->setText(str);
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
