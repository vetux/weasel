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

#ifndef WEASEL_PROCESSTREEITEM_HPP
#define WEASEL_PROCESSTREEITEM_HPP

#include <QStandardItem>

#include "system/systemstatus.hpp"
#include "system/process.hpp"

class ProcessTreeItem : public QStandardItem {
public:
    ProcessTreeItem();

    ProcessTreeItem(const SystemStatus &status, const Process &process);

    ~ProcessTreeItem() override;

    void setProcess(const SystemStatus &status, const Process &process);

    const Process &getProcess();

    const QList<QStandardItem *> &getRowItems();

    void setName(const QString &name);

    void setPid(const QString &pid);

    void setUser(const QString &user);

    void setCommand(const QString &command);

private:
    Process process;

    QList<QStandardItem *> rowItems;
};

#endif //WEASEL_PROCESSTREEITEM_HPP
