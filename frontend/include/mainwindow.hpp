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

#ifndef WEASEL_MAINWINDOW_HPP
#define WEASEL_MAINWINDOW_HPP

#include <QMainWindow>
#include <QTimer>
#include <QAction>

#include "weasel/snapshotgenerator.hpp"

#include "dialog/processpropertiesdialog.hpp"
#include "settings/settingsdialog.hpp"
#include "toolbar/toolbarwidget.hpp"
#include "tabs/tree/processtreewidget.hpp"
#include "tabs/nettable/nettablewidget.hpp"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    explicit MainWindow(int pollingInterval = 1000);

    ~MainWindow() override;

signals:

    void signalSnapshot(const Snapshot &snapshot);

public slots:

    void onPollTimeOut();

    void onActionTriggered(QAction *action);

    void onProcessSignalRequest(Pid_t pid, Thread::Signal signal);

    void onThreadSignalRequest(Pid_t tid, Thread::Signal signal);

    void onProcessPriorityChangeRequest(Pid_t pid, int priority);

    void onThreadPriorityChangeRequest(Pid_t tid, int priority);

    void onOpenPropertiesRequest(Pid_t);

    void onTabChanged(int index);

    void onViewProcess(Pid_t pid);

    void onTerminateProcess(Pid_t pid);

private:
    QTimer pollTimer;

    SnapshotGenerator generator;

    std::map<Pid_t, ProcessPropertiesDialog *> propertyDialogs;

    QTabWidget *tabWidget;
    ProcessTreeWidget *procTree;
    NetTableWidget *netTable;
};

#endif //WEASEL_MAINWINDOW_HPP
