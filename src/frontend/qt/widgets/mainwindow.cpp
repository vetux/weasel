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

#include "frontend/qt/widgets/mainwindow.hpp"

#include <QMenuBar>
#include <QMessageBox>

#include "core/scheduler.hpp"

MainWindow::MainWindow(int pollingInterval) {
    mainWidget = new QWidget();
    toolbar = new ToolbarWidget();
    procTree = new ProcessTreeWidget();

    auto *l = new QVBoxLayout();

    l->addWidget(toolbar);
    l->addWidget(procTree, 1);

    mainWidget->setLayout(l);

    setCentralWidget(mainWidget);

    setupMenuBar();

    refresh();

    connect(&pollTimer, SIGNAL(timeout()), this, SLOT(onPollTimeOut()));
    connect(toolbar, SIGNAL(refreshPressed()), this, SLOT(refreshPressed()));

    connect(procTree,
            SIGNAL(processSignalRequested(const Process &, Signal)),
            this,
            SLOT(processSignalRequested(const Process &, Signal)));
    connect(procTree,
            SIGNAL(threadSignalRequested(const Thread &, Signal)),
            this,
            SLOT(threadSignalRequested(const Thread &, Signal)));
    connect(procTree,
            SIGNAL(processPriorityChangeRequested(const Process &, int)),
            this,
            SLOT(processPriorityChangeRequested(const Process &, int)));
    connect(procTree,
            SIGNAL(threadPriorityChangeRequested(const Thread &, int)),
            this,
            SLOT(threadPriorityChangeRequested(const Thread &, int)));

    pollTimer.start(pollingInterval);
}

MainWindow::~MainWindow() = default;

void MainWindow::setupMenuBar() {
    menuBar()->addMenu("Weasel");
    menuBar()->addMenu("View");
    menuBar()->addMenu("Tools");
    menuBar()->addMenu("Help");
}

void MainWindow::onPollTimeOut() {
    refresh();
}

void MainWindow::refreshPressed() {
    refresh();
}

void MainWindow::refresh() {
    source.refresh();
    procTree->setProcesses(source.getProcesses());
    toolbar->setSystemStatus(source.getSystemStatus());
}

void MainWindow::processSignalRequested(const Process &proc, Signal signal) {
    if (QMessageBox::question(this,
                              QString("Signal Process"),
                              QString(("Do you want to send the signal "
                                       + signalToString(signal)
                                       + " to the process "
                                       + std::to_string(proc.pid)).c_str()))
        != QMessageBox::Yes) {
        return;
    }
    try {
        Scheduler::signal(proc, signal);
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Failed to signal process", e.what());
    }
}

void MainWindow::threadSignalRequested(const Thread &thread, Signal signal) {
    if (QMessageBox::question(this,
                              QString("Signal Thread"),
                              QString(("Do you want to send the signal "
                                       + signalToString(signal)
                                       + " to the thread "
                                       + std::to_string(thread.tid)).c_str()))
        != QMessageBox::Yes) {
        return;
    }
    try {
        Scheduler::signal(thread, signal);
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Failed to signal process", e.what());
    }
}

void MainWindow::processPriorityChangeRequested(const Process &proc, int priority) {
    if (QMessageBox::question(this,
                              QString("Set Process Priority"),
                              QString(("Do you want to set the priority of the process  "
                                       + std::to_string(proc.pid)
                                       + " to the value "
                                       + std::to_string(priority)).c_str()))
        != QMessageBox::Yes) {
        return;
    }
    try {
        Scheduler::setPriority(proc, priority);
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Failed to signal process", e.what());
    }
}

void MainWindow::threadPriorityChangeRequested(const Thread &thread, int priority) {
    if (QMessageBox::question(this,
                              QString("Set Thread Priority"),
                              QString(("Do you want to set the priority of the thread  "
                                       + std::to_string(thread.tid)
                                       + " to the value "
                                       + std::to_string(priority)).c_str()))
        != QMessageBox::Yes) {
        return;
    }
    try {
        Scheduler::setPriority(thread, priority);
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Failed to signal process", e.what());
    }
}
