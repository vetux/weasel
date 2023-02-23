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

#include "mainwindow.hpp"

#include <QMenuBar>
#include <QMessageBox>
#include <QTabWidget>
#include <QApplication>

#include "signalstring.hpp"

MainWindow::MainWindow(int pollingInterval) {
    auto *mainWidget = new QWidget();
    auto *toolbar = new ToolbarWidget();

    auto *l = new QVBoxLayout();

    tabWidget = new QTabWidget();

    procTree = new ProcessTreeWidget();
    netTable = new NetTableWidget();

    exitAction = new QAction();
    exitAction->setText("Exit");

    aboutAction = new QAction();
    aboutAction->setText("About Weasel");

    tabWidget->addTab(procTree, "Process");
    tabWidget->addTab(netTable, "Network");

    l->addWidget(toolbar);
    l->addWidget(tabWidget, 1);

    mainWidget->setLayout(l);

    setCentralWidget(mainWidget);

    auto *menu = menuBar()->addMenu("Weasel");
    menu->addAction(exitAction);

    menu = menuBar()->addMenu("Help");
    menu->addAction(aboutAction);

    connect(&pollTimer, SIGNAL(timeout()), this, SLOT(onPollTimeOut()));

    connect(exitAction, SIGNAL(triggered(bool)), this, SLOT(onActionExit()));
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(onActionAbout()));

    connect(procTree,
            SIGNAL(processSignalRequested(Pid_t, Thread::Signal)),
            this,
            SLOT(onProcessSignalRequest(Pid_t, Thread::Signal)));
    connect(procTree,
            SIGNAL(threadSignalRequested(Pid_t, Thread::Signal)),
            this,
            SLOT(onThreadSignalRequest(Pid_t, Thread::Signal)));
    connect(procTree,
            SIGNAL(processPriorityChangeRequested(Pid_t, int)),
            this,
            SLOT(onProcessPriorityChangeRequest(Pid_t, int)));
    connect(procTree,
            SIGNAL(threadPriorityChangeRequested(Pid_t, int)),
            this,
            SLOT(onThreadPriorityChangeRequest(Pid_t, int)));
    connect(procTree,
            SIGNAL(processPropertiesRequested(Pid_t)),
            this,
            SLOT(onOpenPropertiesRequest(Pid_t)));

    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onTabChanged(int)));

    connect(netTable, SIGNAL(terminateProcess(Pid_t)), this, SLOT(onTerminateProcess(Pid_t)));
    connect(netTable, SIGNAL(viewProcess(Pid_t)), this, SLOT(onViewProcess(Pid_t)));

    connect(this, SIGNAL(signalSnapshot(const Snapshot &)), toolbar, SLOT(onSnapshot(const Snapshot &)));
    connect(this, SIGNAL(signalSnapshot(const Snapshot &)), procTree, SLOT(onSnapshot(const Snapshot &)));
    connect(this, SIGNAL(signalSnapshot(const Snapshot &)), netTable, SLOT(onSnapshot(const Snapshot &)));

    onPollTimeOut();

    pollTimer.start(pollingInterval);
}

MainWindow::~MainWindow() = default;

void MainWindow::onPollTimeOut() {
    emit signalSnapshot(generator.next());
}

void MainWindow::onActionTriggered(QAction *action) {
    if (action->text() == "Preferences") {
        SettingsDialog dialog;
        dialog.exec();
    }
}

void MainWindow::onProcessSignalRequest(Pid_t pid, Thread::Signal signal) {
    if (QMessageBox::question(this,
                              QString("Signal Process"),
                              QString(("Do you want to send "
                                       + signalToString(signal)
                                       + " to "
                                       + std::to_string(pid)
                                       + " ?").c_str()))
        != QMessageBox::Yes) {
        return;
    }
    //TODO: Handle cases where a thread / process is newly spawned with the target pid the second the call to sendSignal is made.
    try {
        Thread::sendSignal(pid, signal);
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Failed to signal process", e.what());
    }
}

void MainWindow::onThreadSignalRequest(Pid_t tid, Thread::Signal signal) {
    if (QMessageBox::question(this,
                              QString("Signal Thread"),
                              QString(("Do you want to send "
                                       + signalToString(signal)
                                       + " to thread "
                                       + std::to_string(tid)
                                       + "?").c_str()))
        != QMessageBox::Yes) {
        return;
    }
    try {
        Thread::sendSignal(tid, signal);
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Failed to signal process", e.what());
    }
}

void MainWindow::onProcessPriorityChangeRequest(Pid_t pid, int priority) {
    if (QMessageBox::question(this,
                              QString("Set Process Priority"),
                              QString(("Do you want to set the priority of the process  "
                                       + std::to_string(pid)
                                       + " to the value "
                                       + std::to_string(priority)).c_str()))
        != QMessageBox::Yes) {
        return;
    }
    try {
        Thread::setPriority(pid, priority);
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Failed to signal process", e.what());
    }
}

void MainWindow::onThreadPriorityChangeRequest(Pid_t tid, int priority) {
    if (QMessageBox::question(this,
                              QString("Set Thread Priority"),
                              QString(("Do you want to set the priority of the thread  "
                                       + std::to_string(tid)
                                       + " to the value "
                                       + std::to_string(priority)).c_str()))
        != QMessageBox::Yes) {
        return;
    }
    try {
        Thread::setPriority(tid, priority);
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Failed to signal process", e.what());
    }
}

void MainWindow::onOpenPropertiesRequest(Pid_t pid) {
    //Check if a dialog already exists for the passed process
    auto it = propertyDialogs.find(pid);
    if (it != propertyDialogs.end()) {
        it->second->activateWindow();
    } else {
        generator.setProcessReadFlags(pid, READ_PROCESS_IO | READ_PROCESS_FD | READ_PROCESS_THREADS | READ_THREAD_IO);

        auto *dialog = new ProcessPropertiesDialog(this, pid);
        connect(dialog,
                &QDialog::finished,
                [this, pid](int) {
                    // Because there seems to be differences in behaviour across qt versions
                    // which could cause the finished event to be called twice because we call finished manually
                    // in the closeEvent handler of the window we check if the dialog was already erased.
                    if (propertyDialogs.find(pid) != propertyDialogs.end())
                        propertyDialogs.erase(pid);

                    generator.clearProcessReadFlags(pid);
                });
        propertyDialogs.insert(std::pair<Pid_t, ProcessPropertiesDialog *>(pid, dialog));
        connect(this, SIGNAL(signalSnapshot(const Snapshot &)), dialog, SLOT(onSnapshot(const Snapshot &)));
        onPollTimeOut(); //Cause a poll timeout to populate the dialog before showing.
        dialog->show();
    }
}

void MainWindow::onTabChanged(int index) {
    if (index > 0) {
        generator.setDefaultReadFlags(READ_THREAD_IO | READ_PROCESS_FD | READ_PROCESS_THREADS | READ_THREAD_IO);
    } else {
        generator.setDefaultReadFlags(READ_NONE);
    }
    onPollTimeOut();
}

void MainWindow::onViewProcess(Pid_t pid) {
    tabWidget->setCurrentIndex(0);
    procTree->selectProcess(pid);
}

void MainWindow::onTerminateProcess(Pid_t pid) {
    onProcessSignalRequest(pid, Thread::SIGNAL_SIGTERM);
}

void MainWindow::onActionExit() {
    close();
}

void MainWindow::onActionAbout() {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("About Weasel");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText("<h1>" + QApplication::applicationDisplayName() + " " + QApplication::applicationVersion() + "</h1>\n"
                   + "Copyright (C) 2023 Julian Zampiccoli\n"
                   +
                   "<p>Weasel comes with ABSOLUTELY NO WARRANTY; This is free software, and you are welcome to redistribute it under certain conditions; check <a href=\"https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt\">GPL-2.0</a> for details</p>\n"
                   + "<p><a href=\"https://www.github.com/vetux/weasel\">Source Code</a></p>");
    msgBox.exec();
}
