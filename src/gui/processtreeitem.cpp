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

#include "gui/processtreeitem.hpp"

#include "os/users.hpp"

QList<QStandardItem *> createRow(const Process &proc) {
    QList<QStandardItem *> ret;
    ret.append(new QStandardItem(QString("%0").arg(proc.threads.at(0).pid)));
    ret.append(new QStandardItem(QString("%0").arg(Users::getUserName(proc.threads.at(0).uid).c_str())));
    ret.append(new QStandardItem(QString("%0").arg(proc.commandLine.c_str())));
    return ret;
}

ProcessTreeItem::ProcessTreeItem(const Process &process)
        : process(process),
          rowItems(createRow(process)),
          QStandardItem(QString("%0").arg(process.threads.at(0).comm.c_str())) {
    rowItems.insert(0, this);
}

ProcessTreeItem::~ProcessTreeItem() = default;

const Process &ProcessTreeItem::getProcess() {
    return process;
}

const QList<QStandardItem *> &ProcessTreeItem::getRow() {
    return rowItems;
}
