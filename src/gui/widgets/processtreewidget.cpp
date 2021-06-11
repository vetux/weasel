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

#include "gui/widgets/processtreewidget.hpp"

#include <QVBoxLayout>
#include <QHeaderView>
#include <QMenu>
#include <set>

#include "os/users.hpp"

std::vector<Pid_t> getPidRecursive(QStandardItem &item) {
    std::vector<Pid_t> ret;
    auto *node = dynamic_cast<ProcessTreeItem *>(&item);
    if (node != nullptr) {
        ret.emplace_back(node->getProcess().pid);
    }

    QStandardItem *child = item.child(0);
    for (int row = 0; child != nullptr; child = item.child(row++)) {
        auto tmp = getPidRecursive(*child);
        for (auto t : tmp)
            ret.emplace_back(t);
    }

    return ret;
}

bool findNodeRecursive(QStandardItem &item, Pid_t pid, ProcessTreeItem *&output) {
    auto *node = dynamic_cast<ProcessTreeItem *>(&item);
    if (node != nullptr) {
        if (node->getProcess().pid == pid) {
            output = node;
            return true;
        }
    }

    QStandardItem *child = item.child(0);
    for (int row = 0; child != nullptr; child = item.child(row++)) {
        if (findNodeRecursive(*child, pid, output)) {
            return true;
        }
    }

    return false;
}

ProcessTreeWidget::ProcessTreeWidget(QWidget *parent) : QWidget(parent) {
    setLayout(new QVBoxLayout());
    treeView = new QTreeView();

    treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeView->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

    model.setColumnCount(4);
    model.setHorizontalHeaderLabels({"Name", "PID", "User", "Command"});

    treeView->setModel(&model);

    connect(treeView, SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(doubleCLicked(const QModelIndex &)));

    connect(treeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(clicked()));

    connect(treeView,
            SIGNAL(customContextMenuRequested(const QPoint &)),
            this,
            SLOT(customContextMenu(const QPoint &)));

    layout()->addWidget(treeView);
}

ProcessTreeWidget::~ProcessTreeWidget() {

}

void ProcessTreeWidget::setProcesses(const std::map<Pid_t, Process> &p) {
    auto *tree = model.invisibleRootItem();

    std::vector<Pid_t> deadProcesses;

    for (auto &pair : p) {
        ProcessTreeItem *n;
        if (findNodeRecursive(*tree, pair.first, n)) {
            if (n->getProcess().threads.at(0).starttime != pair.second.threads.at(0).starttime) {
                deadProcesses.emplace_back(pair.first);
            }
        } else {
            deadProcesses.emplace_back(pair.first);
        }
    }

    auto pids = getPidRecursive(*tree);
    for (auto pid : pids) {
        if (p.find(pid) == p.end()) {
            deadProcesses.emplace_back(pid);
        }
    }

    for (auto deadPid : deadProcesses) {
        if (deadPid == 0)
            continue;
        ProcessTreeItem *n;
        if (findNodeRecursive(*tree, deadPid, n)) {
            n->parent()->removeRow(n->row());
        }
    }

    std::map<Pid_t, Pid_t> newProcesses;

    for (auto &pair : p) {
        ProcessTreeItem *n;
        if (!findNodeRecursive(*tree, pair.first, n)) {
            newProcesses[pair.first] = pair.second.threads.at(0).ppid;
        }
    }

    std::vector<Pid_t> del;
    while (!newProcesses.empty()) {
        for (auto pair : newProcesses) {
            if (pair.second == 0) {
                auto *item = new ProcessTreeItem(p.at(pair.first));
                tree->appendRow(item->getRow());
                del.emplace_back(pair.first);
            } else {
                ProcessTreeItem *parent;
                if (findNodeRecursive(*tree, pair.second, parent)) {
                    auto *item = new ProcessTreeItem(p.at(pair.first));
                    parent->appendRow(item->getRow());
                    del.emplace_back(pair.first);
                }
            }
        }

        for (auto pid : del) {
            newProcesses.erase(pid);
        }

        del.clear();
    }
}

void ProcessTreeWidget::clicked() {

}

void ProcessTreeWidget::doubleCLicked(const QModelIndex &index) {

}

void ProcessTreeWidget::customContextMenu(const QPoint &pos) {
    QModelIndex index = treeView->indexAt(pos);
    if (index.isValid()) {
        auto *contextMenu = new QMenu(treeView);
        contextMenu->addAction("Test");
        contextMenu->addAction("Test");
        contextMenu->addMenu("TestMenu")->addAction("Test");
        contextMenu->exec(treeView->viewport()->mapToGlobal(pos));
        delete contextMenu;
    }
}
