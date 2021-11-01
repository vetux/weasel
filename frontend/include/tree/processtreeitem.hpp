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

#include <qt5/QtGui/QStandardItem>

#include "weasel/process.hpp"

class ProcessTreeItem : public QStandardItem {
public:
    explicit ProcessTreeItem(Pid_t pid);

    ~ProcessTreeItem() override;

    Pid_t getPid() const;

    const QList<QStandardItem *> &getRowItems();

public slots:

    void setName(const QString &name);

    void setPid(const QString &pid);

    void setUser(const QString &user);

    void setCpu(const QString &cpu);

    void setVirtual(const QString &mem);

    void setResident(const QString &mem);

    void setShared(const QString &mem);

    void setCommand(const QString &command);

private:
    Pid_t pid;

    QList<QStandardItem *> rowItems;
};

#endif //WEASEL_PROCESSTREEITEM_HPP
