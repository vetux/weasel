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

#include "widgets/processtreewidget.hpp"

#include <QVBoxLayout>
#include <QHeaderView>

#include "os/users.hpp"

QList<QStandardItem *> getRow(const std::map<Pid_t, Process> &p, const Thread &thread) {
    QList<QStandardItem *> ret;
    ret.append(new QStandardItem(QString("%0").arg(thread.comm.c_str())));
    ret.append(new QStandardItem(QString("%0").arg(thread.pid)));
    ret.append(new QStandardItem(QString("%0").arg(Users::getUserName(thread.uid).c_str())));
    ret.append(new QStandardItem(QString("%0").arg(p.at(thread.pid).commandLine.c_str())));
    return ret;
}

ProcessTreeWidget::ProcessTreeWidget(QWidget *parent) : QWidget(parent) {
    setLayout(new QVBoxLayout());
    treeView = new QTreeView();

    model.setColumnCount(4);
    model.setHorizontalHeaderLabels({"Name", "PID", "User", "Command"});

    treeView->setModel(&model);

    connect(treeView, SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(doubleCLicked(const QModelIndex &)));

    connect(treeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(clicked()));

    layout()->addWidget(treeView);
}

ProcessTreeWidget::~ProcessTreeWidget() {

}

void ProcessTreeWidget::setProcesses(const std::map<Pid_t, Process> &p) {
    bool deadProcesses = false;
    for (auto &existingProcess : processes) {
        if (p.find(existingProcess.first) == p.end()) {
            deadProcesses = true;
            break;
        }
    }

    std::vector<Pid_t> newProcesses;

    if (deadProcesses) {
        processes.clear();
        itemMapping.clear();
        model.clear();
        model.setColumnCount(4);
        model.setHorizontalHeaderLabels({"Name", "PID", "User", "Command"});
        for (auto &pair : p)
            newProcesses.emplace_back(pair.first);
    } else {
        for (auto &pair : p) {
            auto it = processes.find(pair.first);
            if (processes.find(pair.first) == processes.end()) {
                newProcesses.emplace_back(pair.first);
            } else {
                auto existingProcess = processes.at(pair.first);
                if (existingProcess.threads.at(0).starttime != pair.second.threads.at(0).starttime) {
                    processes.erase(pair.first);
                    delete itemMapping.at(pair.first);
                    itemMapping.erase(pair.first);
                    newProcesses.emplace_back(pair.first);
                }
            }
        }
    }

    std::vector<Pid_t> del;

    QStandardItem *itemsRoot = model.invisibleRootItem();

    while (!newProcesses.empty()) {
        for (auto &pid : newProcesses) {
            auto proc = p.at(pid);
            auto ppid = proc.threads.at(0).ppid;
            if (ppid > 0) {
                if (itemMapping.find(ppid) != itemMapping.end()) {
                    QList<QStandardItem *> l = getRow(p, proc.threads.at(0));
                    itemMapping.at(ppid)->appendRow(l);
                    itemMapping[pid] = l[0];
                    del.emplace_back(pid);
                }
            } else {
                QList<QStandardItem *> l = getRow(p, proc.threads.at(0));
                itemsRoot->appendRow(l);
                itemMapping[pid] = l[0];
                del.emplace_back(pid);
            }
        }

        for (auto pid : del)
            newProcesses.erase(std::find(newProcesses.begin(), newProcesses.end(), pid));

        del.clear();
    }

    processes = p;
}

void ProcessTreeWidget::clicked() {

}

void ProcessTreeWidget::doubleCLicked(const QModelIndex &index) {

}
