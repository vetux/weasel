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

#include "dialog/processpropertiesdialog.hpp"

#include <QVBoxLayout>

#include "byteformatting.hpp"

ProcessPropertiesDialog::ProcessPropertiesDialog(QWidget *parent, Pid_t pid)
        : QDialog(parent), pid(pid) {
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

    setWindowTitle(("Properties " + std::to_string(pid)).c_str());
}

Pid_t ProcessPropertiesDialog::getPid() const {
    return pid;
}

void ProcessPropertiesDialog::onSnapshot(const Snapshot &snapshot) {
    auto it = snapshot.processes.find(pid);
    if (it == snapshot.processes.end()) {
        processDead = true;
    }

    if (processDead) {
        tabDisk->setOpenFiles({});
        tabDisk->setReadBytes("");
        tabDisk->setWriteBytes("");
        tabDisk->setReadRate("");
        tabDisk->setWriteRate("");

        tabGeneral->setName("");
        tabGeneral->setPid("");
        tabGeneral->setUserId("");
        tabGeneral->setUserName("");
        tabGeneral->setCommandLine("");
        tabGeneral->setExecutablePath("");
        tabGeneral->setRootDirectory("");
        tabGeneral->setEnvironment({});

        tabNetwork->clearSockets();
        tabNetwork->setReadBytes("");
        tabNetwork->setWriteBytes("");
        tabNetwork->setReadRate("");
        tabNetwork->setWriteRate("");
    } else {
        auto &process = it->second;

        tabDisk->setOpenFiles(process.files);
        tabDisk->setReadBytes(getFormattedSizeString(process.rchar));
        tabDisk->setWriteBytes(getFormattedSizeString(process.wchar));
        tabDisk->setReadRate(getFormattedRateString(process.rate_rchar));
        tabDisk->setWriteRate(getFormattedRateString(process.rate_wchar));

        tabGeneral->setName(process.mainThread().comm);
        tabGeneral->setPid(std::to_string(process.pid));
        tabGeneral->setUserId(std::to_string(process.uid));
        tabGeneral->setUserName(process.userName);
        tabGeneral->setCommandLine(process.commandLine);
        tabGeneral->setExecutablePath(process.executablePath);
        tabGeneral->setRootDirectory(process.rootDirectory);
        tabGeneral->setEnvironment(process.environ);


        for (auto &pSocket: snapshot.deadSockets) {
            tabNetwork->removeSocket(pSocket.second);
        }

        for (auto &pSocket: process.sockets) {
            tabNetwork->setSocket(pSocket);
        }

        tabNetwork->setReadBytes(getFormattedSizeString(process.network_read_bytes));
        tabNetwork->setWriteBytes(getFormattedSizeString(process.network_write_bytes));
        tabNetwork->setReadRate(getFormattedRateString(process.network_read_rate));
        tabNetwork->setWriteRate(getFormattedRateString(process.network_write_rate));
    }
}

void ProcessPropertiesDialog::closeEvent(QCloseEvent *event) {
    //Because qt does not emit finished event for non modal dialogs we hook the close event and call finished manually.
    finished(QDialog::DialogCode::Accepted);
}
