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

#ifndef WEASEL_PROCESSTREEWIDGET_HPP
#define WEASEL_PROCESSTREEWIDGET_HPP

#include <set>

#include <qt5/QtWidgets/QWidget>
#include <qt5/QtWidgets/QTreeView>
#include <qt5/QtGui/QStandardItemModel>

#include "weasel/process.hpp"
#include "weasel/snapshot.hpp"

#include "processtreeitem.hpp"

class ProcessTreeWidget : public QWidget {
Q_OBJECT
public:
    explicit ProcessTreeWidget(QWidget *parent = nullptr);

    ~ProcessTreeWidget() override;

public slots:

    void onSnapshot(const Snapshot &snapshot);

signals:

    void processSignalRequested(Pid_t pid, Thread::Signal signal);

    void threadSignalRequested(Pid_t tid, Thread::Signal signal);

    void processPriorityChangeRequested(Pid_t pid, int priority);

    void threadPriorityChangeRequested(Pid_t tid, int priority);

    void processPropertiesRequested(Pid_t pid);

private slots:

    void clicked();

    void doubleClicked(const QModelIndex &index);

    void customContextMenu(const QPoint &pos);

private:
    QStandardItemModel model;

    QTreeView *treeView;

    std::map<Pid_t, ProcessTreeItem *> items;
};

#endif //WEASEL_PROCESSTREEWIDGET_HPP
