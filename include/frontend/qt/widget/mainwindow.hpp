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

#include "frontend/qt/widget/processtreewidget.hpp"
#include "frontend/qt/widget/toolbarwidget.hpp"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    explicit MainWindow(int pollingInterval = 1000);

    ~MainWindow() override;

private slots:

    void onPollTimeOut();

    void processSignalRequested(const Process &proc, Signal signal);

    void threadSignalRequested(const Thread &thread, Signal signal);

    void processPriorityChangeRequested(const Process &proc, int priority);

    void threadPriorityChangeRequested(const Thread &thread, int priority);

    void processPropertiesRequested(const Process &proc);

private:
    void setupMenuBar();

    void refresh();

    QTimer pollTimer;

    QWidget *mainWidget;

    ToolbarWidget *toolbar;
    ProcessTreeWidget *procTree;

    SystemStatus prevStatus;
    std::map<Pid_t, Process> prevProc;

    std::set<ProcessPropertiesDialog *> dialogs;
};

#endif //WEASEL_MAINWINDOW_HPP
