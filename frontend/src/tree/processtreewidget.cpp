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

#include "tree/processtreewidget.hpp"

#include <QVBoxLayout>
#include <QHeaderView>
#include <QMenu>

#include <set>
#include <limits>

#include "byteformatting.hpp"
#include "strutil.hpp"
#include "signalstring.hpp"

static std::vector<ProcessTreeItem *> getItemsRecursive(Pid_t pid, const std::map<Pid_t, ProcessTreeItem *> &items) {
    std::vector<ProcessTreeItem *> ret;

    auto *item = items.at(pid);

    ret.emplace_back(item);

    for (int i = 0; i < std::numeric_limits<int>::max(); i++) {
        auto *child = dynamic_cast<ProcessTreeItem *>(item->child(i));
        if (child == nullptr)
            break;

        ret.emplace_back(child);
        auto recurse = getItemsRecursive(child->getPid(), items);
        for (auto recurseItem: recurse) {
            ret.emplace_back(recurseItem);
        }
    }

    return ret;
}

static void applyProcessToItem(ProcessTreeItem &item, const Process &process) {
    item.setName(QString("%0").arg(process.mainThread().comm.c_str()));
    item.setPid(QString("%0").arg(process.mainThread().pid));
    item.setUser(QString("%0").arg(process.userName.c_str()));

    float cpu = process.usage * 100;
    if (cpu == 0)
        item.setCpu("");
    else
        item.setCpu(QString("%0").arg(cpu, 0, 'f', 2));

    item.setVirtual(QString("%0").arg(getFormattedSizeString(process.vsize).c_str()));
    item.setResident(QString("%0").arg(getFormattedSizeString(process.resident * MemoryState::getPageSize()).c_str()));
    item.setShared(QString("%0").arg(getFormattedSizeString(process.shared * MemoryState::getPageSize()).c_str()));

    std::string c = process.commandLine;
    replace(c, std::string("\0", 1), " ");
    item.setCommand(QString("%0").arg(c.c_str()));
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

    model.setColumnCount(8);
    model.setHorizontalHeaderLabels({"Name", "PID", "User", "Cpu", "Virtual", "Resident", "Shared", "Command"});

    treeView->setModel(&model);
    treeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

    connect(treeView,
            SIGNAL(doubleClicked(const QModelIndex &)),
            this,
            SLOT(doubleClicked(const QModelIndex &)));

    connect(treeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(clicked()));

    connect(treeView,
            SIGNAL(customContextMenuRequested(const QPoint &)),
            this,
            SLOT(customContextMenu(const QPoint &)));

    layout()->addWidget(treeView);
}

ProcessTreeWidget::~ProcessTreeWidget() = default;

void ProcessTreeWidget::onSnapshot(const Snapshot &snapshot) {
    auto *tree = model.invisibleRootItem();

    //Destroy dead process items
    for (auto &deadProcess: snapshot.deadProcesses) {
        auto deadPid = deadProcess.first;

        auto it = items.find(deadPid);

        //Check if item was already removed because the inner loop recursively erases all child items
        if (it != items.end()) {
            auto rItems = getItemsRecursive(it->second->getPid(), items);
            for (auto rItem: rItems) {
                items.erase(rItem->getPid()); //Remove item pid mapping
            }

            it->second->parent()->removeRow(it->second->row()); // Deallocate the item objects
        }
    }

    //Get new process and parent ids
    std::map<Pid_t, Pid_t> newProcesses;
    for (auto &spawnedProcess: snapshot.spawnedProcesses) {
        assert(items.find(spawnedProcess) == items.end());
        newProcesses[spawnedProcess] = snapshot.processes.at(spawnedProcess).ppid;
    }

    // Bruteforce method to create items in hierarchical order, wastes cycles a better approach would be sorting.
    std::vector<Pid_t> del;
    while (!newProcesses.empty()) {
        for (auto pair: newProcesses) {
            if (pair.second == 0) {
                // No Parent
                auto *item = new ProcessTreeItem(pair.first);
                tree->appendRow(item->getRowItems());
                del.emplace_back(pair.first);
                items[pair.first] = item;
            } else {
                // Parent
                auto it = items.find(pair.second);

                //Check if the parent was already created
                if (it != items.end()) {
                    auto parent = it->second;
                    auto *item = new ProcessTreeItem(pair.first); // Allocate item object
                    parent->appendRow(item->getRowItems());
                    del.emplace_back(pair.first);
                    items[pair.first] = item; // Create item pid mapping
                }
            }
        }

        for (auto pid: del) {
            newProcesses.erase(pid);
        }

        del.clear();
    }

    //Update item values
    for (auto &pair: snapshot.processes) {
        applyProcessToItem(*items.at(pair.first), pair.second);
    }
}

void ProcessTreeWidget::clicked() {

}

void ProcessTreeWidget::doubleClicked(const QModelIndex &index) {
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
            for (int i = Thread::SIGNAL_SIGHUP; i < Thread::SIGNAL_SIGTTOU; i++) {
                menu->addAction(signalToString(static_cast<Thread::Signal>(i)).c_str());
            }

            contextMenu->addSeparator();
            contextMenu->addAction("Expand All");
            contextMenu->addAction("Collapse All");

            contextMenu->addSeparator();
            contextMenu->addAction("Properties");

            connect(contextMenu,
                    &QMenu::triggered,
                    [=](QAction *action) {
                        if (action->text() == "Terminate") {
                            emit processSignalRequested(item->getPid(), Thread::SIGNAL_SIGTERM);
                        } else if (action->text() == "Signal") {
                            emit processSignalRequested(item->getPid(), stringToSignal(action->text().toStdString()));
                        } else if (action->text() == "Expand All") {
                            treeView->expandRecursively(item->index());
                        } else if (action->text() == "Collapse All") {
                            auto recursiveItems = getItemsRecursive(item->getPid(), items);
                            for (auto rItem: recursiveItems) {
                                treeView->collapse(rItem->index());
                                treeView->update();
                            }
                        } else if (action->text() == "Properties") {
                            emit processPropertiesRequested(item->getPid());
                        } else {
                            assert(false);
                        }
                    }
            );

            contextMenu->exec(treeView->viewport()->mapToGlobal(pos));

            delete contextMenu;
        }
    }

}