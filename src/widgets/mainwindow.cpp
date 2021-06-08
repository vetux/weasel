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

#include "widgets/mainwindow.hpp"

#include <QMenuBar>

MainWindow::MainWindow(int pollingInterval) {
    mainWidget = new QWidget();
    auto *l = new QVBoxLayout();

    toolbar = new ToolbarWidget();
    procTree = new ProcessTreeWidget();

    l->addWidget(toolbar);
    l->addWidget(procTree, 1);

    mainWidget->setLayout(l);

    setCentralWidget(mainWidget);

    setupMenuBar();

    onPollTimeOut();

    connect(&pollTimer, SIGNAL(timeout()), this, SLOT(onPollTimeOut()));
    pollTimer.start(pollingInterval);
}

MainWindow::~MainWindow() {}

void MainWindow::setupMenuBar() {
    menuBar()->addMenu("Weasel");
    menuBar()->addMenu("View");
    menuBar()->addMenu("Tools");
    menuBar()->addMenu("Help");
}

void MainWindow::onPollTimeOut() {
    sched.refresh();
    procTree->setProcesses(sched.getProcesses());
    toolbar->setMemory(sched.getMemory());
}
