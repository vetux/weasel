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

#include "frontend/qt/widget/processtreewidget.hpp"

#include <QVBoxLayout>
#include <QHeaderView>
#include <QMenu>

#include <set>

#include "system/user.hpp"

#include "frontend/qt/widget/processpropertiesdialog.hpp"

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

std::map<Pid_t, ProcessTreeItem *> getItemsRecursive(QStandardItem &item) {
    std::map<Pid_t, ProcessTreeItem *> ret;

    auto *node = dynamic_cast<ProcessTreeItem *>(&item);
    if (node != nullptr) {
        ret[node->getProcess().pid] = node;
    }

    QStandardItem *child = item.child(0);
    for (int row = 0; child != nullptr; child = item.child(row++)) {
        auto r = getItemsRecursive(*child);
        for (auto pair : r) {
            ret[pair.first] = pair.second;
        }
    }

    return ret;
}

ProcessTreeWidget::ProcessTreeWidget(QWidget *parent) : QWidget(parent) {
    setLayout(new QVBoxLayout());

    layout()->setMargin(0);

    treeView = new QTreeView();

    treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeView->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    treeView->setExpandsOnDoubleClick(false);

    treeView->setStyleSheet("QTreeView::item { padding: 6px; }");

    model.setColumnCount(4);
    model.setHorizontalHeaderLabels({"Name", "PID", "User", "Command"});

    treeView->setModel(&model);

    connect(treeView,
            SIGNAL(doubleClicked(const QModelIndex &)),
            this,
            SLOT(doubleCLicked(const QModelIndex &)));

    connect(treeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(clicked()));

    connect(treeView,
            SIGNAL(customContextMenuRequested(const QPoint &)),
            this,
            SLOT(customContextMenu(const QPoint &)));

    layout()->addWidget(treeView);
}

ProcessTreeWidget::~ProcessTreeWidget() = default;

void ProcessTreeWidget::setContents(const SystemStatus &status, const std::map<Pid_t, Process> &processes) {
    auto *tree = model.invisibleRootItem();

    std::map<Pid_t, ProcessTreeItem *> items = getItemsRecursive(*tree);
    std::set<Pid_t> deadProcesses;

    for (auto &pair : processes) {
        auto it = items.find(pair.first);
        if (it != items.end()) {
            if (it->second->getProcess().threads.at(0).starttime != pair.second.threads.at(0).starttime) {
                deadProcesses.insert(pair.first);
            }
        }
    }

    for (auto pair : items) {
        if (processes.find(pair.first) == processes.end()) {
            deadProcesses.insert(pair.first);
        }
    }

    for (auto deadPid : deadProcesses) {
        if (deadPid == 0)
            continue;
        auto it = items.find(deadPid);
        if (it != items.end()) {
            auto pids = getPidRecursive(*it->second);
            for (Pid_t pid : pids) {
                items.erase(pid);
            }
            it->second->parent()->removeRow(it->second->row());
        }
    }

    std::map<Pid_t, Pid_t> newProcesses;
    for (auto &pair : processes) {
        auto it = items.find(pair.first);
        if (it == items.end()) {
            newProcesses[pair.first] = pair.second.threads.at(0).ppid;
        } else {
            it->second->setProcess(status, pair.second);
        }
    }

    std::vector<Pid_t> del;
    while (!newProcesses.empty()) {
        for (auto pair : newProcesses) {
            if (pair.second == 0) {
                auto *item = new ProcessTreeItem(status, processes.at(pair.first));
                tree->appendRow(item->getRowItems());
                del.emplace_back(pair.first);
            } else {
                ProcessTreeItem *parent;
                if (findNodeRecursive(*tree, pair.second, parent)) {
                    auto *item = new ProcessTreeItem(status, processes.at(pair.first));
                    parent->appendRow(item->getRowItems());
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
    auto row = model.index(index.row(), 0, index.parent());
    if (treeView->isExpanded(row))
        treeView->collapse(row);
    else
        treeView->expand(row);
}

void ProcessTreeWidget::customContextMenu(const QPoint &pos) {
    QModelIndex index = treeView->indexAt(pos);

    if (index.isValid()) {
        auto *item = dynamic_cast<ProcessTreeItem *>(model.itemFromIndex(model.index(index.row(), 0, index.parent())));
        if (item != nullptr) {
            auto *contextMenu = new QMenu(treeView);

            contextMenu->addAction("Terminate");

            auto *menu = contextMenu->addMenu("Signal");
            for (int i = SIGNAL_SIGHUP; i < SIGNAL_SIGTTOU; i++) {
                menu->addAction(signalToString(static_cast<Signal>(i)).c_str());
            }

            contextMenu->addSeparator();
            contextMenu->addAction("Properties");

            // Seems to compile regardless of static type related compile error shown in IDE when using lambda as slot.
            connect(contextMenu,
                    &QMenu::triggered,
                    [=](QAction *action) {
                        if (action->text() == "Terminate") {
                            emit processSignalRequested(item->getProcess(), SIGNAL_SIGTERM);
                        } else if (action->text() == "Properties") {
                            ProcessPropertiesDialog dialog(this, item->getProcess());
                            dialog.show();
                            dialog.exec();
                        } else {
                            emit processSignalRequested(item->getProcess(),
                                                        stringToSignal(action->text().toStdString()));
                        }
                    });

            contextMenu->exec(treeView->viewport()->mapToGlobal(pos));

            delete contextMenu;
        }
    }
}