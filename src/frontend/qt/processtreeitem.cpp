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

#include "frontend/qt/processtreeitem.hpp"

#include "system/users.hpp"

ProcessTreeItem::ProcessTreeItem() = default;

ProcessTreeItem::ProcessTreeItem(const SystemStatus &status, const Process &process)
        : process(process),
          rowItems(),
          QStandardItem() {
    rowItems.append(this);

    for (int i = 0; i < 4; i++)
        rowItems.append(new QStandardItem());

    setProcess(status, process);
}

ProcessTreeItem::~ProcessTreeItem() = default;

void ProcessTreeItem::setProcess(const SystemStatus &s, const Process &p) {
    process = p;
    setName(QString("%0").arg(process.threads.at(0).comm.c_str()));
    setPid(QString("%0").arg(process.threads.at(0).pid));
    setUser(QString("%0").arg(Users::getUserName(process.threads.at(0).uid).c_str()));
    setCommand(QString("%0").arg(process.commandLine.c_str()));
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

void ProcessTreeItem::setCommand(const QString &command) {
    rowItems.at(3)->setText(command);
}
