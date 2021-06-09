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

#include <QWidget>
#include <QTreeView>
#include <QStandardItemModel>

#include "os/process.hpp"
#include "os/signal.hpp"

class ProcessTreeWidget : public QWidget {
Q_OBJECT
signals:

    void processSignalRequested(const Process &proc, Signal signal);

    void threadSignalRequested(const Thread &thread, Signal signal);

    void processPriorityChangeRequested(const Process &proc, int priority);

    void threadPriorityChangeRequested(const Thread &thread, int priority);

public:
    explicit ProcessTreeWidget(QWidget *parent = nullptr);

    ~ProcessTreeWidget() override;

    void setProcesses(const std::map<Pid_t, Process> &processes);

private slots:

    void clicked();

    void doubleCLicked(const QModelIndex &index);

    void customContextMenu(const QPoint &pos);

private:
    QStandardItemModel model;
    
    QTreeView *treeView;

    std::map<Pid_t, QStandardItem *> itemMapping;
    std::map<Pid_t, Process> processes;
};

#endif //WEASEL_PROCESSTREEWIDGET_HPP
