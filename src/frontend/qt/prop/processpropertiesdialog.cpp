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

#include "frontend/qt/prop/processpropertiesdialog.hpp"

#include <QVBoxLayout>

ProcessPropertiesDialog::ProcessPropertiesDialog(QWidget *parent)
        : QDialog(parent) {
    tabWidget = new QTabWidget(this);
    tabGeneral = new GeneralTab(tabWidget);
    tabThreads = new ThreadsTab(tabWidget);
    tabDisk = new DiskTab(tabWidget);
    tabNetwork = new NetworkTab(tabWidget);

    tabWidget->addTab(tabGeneral, "General");
    tabWidget->addTab(tabThreads, "Threads");
    tabWidget->addTab(tabDisk, "Disk");
    tabWidget->addTab(tabNetwork, "Network");

    setLayout(new QVBoxLayout());
    layout()->addWidget(tabWidget);

    layout()->setMargin(6);

    resize(640, 320);
}

void ProcessPropertiesDialog::setData(const SystemStatus &status,
                                      const SystemStatus &prevStatus,
                                      const Process &proc,
                                      const Process &prevProc) {
    process = proc;
    processDead = false;

    setWindowTitle(("Properties " + std::to_string(process.pid)).c_str());

    auto &p = proc;
    auto &pv = prevProc;
    tabGeneral->setData(status, prevStatus, p, pv);
    tabThreads->setData(status, prevStatus, p, pv);
    tabDisk->setData(status, prevStatus, p, pv);
    tabNetwork->setData(status, prevStatus, p, pv);
}

void ProcessPropertiesDialog::updateData(const SystemStatus &status,
                                         const SystemStatus &prevStatus,
                                         const std::map<Pid_t, Process> &processes,
                                         const std::map<Pid_t, Process> &prevProc) {
    if (processDead || processes.find(process.pid) == processes.end()) {
        if (!processDead) {
            processDead = true;
            setWindowTitle(windowTitle() + " (Dead)");
            tabGeneral->clearData();
            tabThreads->clearData();
            tabDisk->clearData();
            tabNetwork->clearData();
        }
    } else {
        auto &p = processes.at(process.pid);
        auto &pv = prevProc.at(process.pid);
        tabGeneral->updateData(status, prevStatus, p, pv);
        tabThreads->updateData(status, prevStatus, p, pv);
        tabDisk->updateData(status, prevStatus, p, pv);
        tabNetwork->updateData(status, prevStatus, p, pv);
    }
}
