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
#include <QMenu>
#include <set>

#include "os/users.hpp"

struct TreeNode {
    QStandardItem *parent;
    std::vector<QStandardItem *> items;
    Process process;
    std::vector<TreeNode> children;
};

bool findNodeRecursive(const TreeNode &tree, Pid_t pid, TreeNode &output) {
    if (tree.process.pid == pid) {
        output = tree;
        return true;
    }

    for (auto &child : tree.children) {
        if (findNodeRecursive(child, pid, output))
            return true;
    }

    return false;
}

TreeNode createNodeRecursive(QStandardItem *parent,
                             const Process &process,
                             const std::map<Pid_t, Process> &flatMap,
                             const std::map<Pid_t, std::vector<QStandardItem *>> &itemMapping) {
    TreeNode ret;
    ret.parent = parent;
    ret.process = process;
    auto it = itemMapping.find(process.pid);
    if (it != itemMapping.end())
        ret.items = it->second;

    for (auto &pair : flatMap) {
        if (pair.second.threads.at(0).ppid == process.pid) {
            ret.children.emplace_back(
                    createNodeRecursive(itemMapping.at(process.pid).at(0),
                                        pair.second,
                                        flatMap,
                                        itemMapping));
        }
    }

    return ret;
}

void eraseNodeRecursive(const TreeNode &node,
                        std::map<Pid_t, Process> &processes,
                        QStandardItemModel &model) {
    for (auto &child : node.children) {
        eraseNodeRecursive(child, processes, model);
    }

    node.items.at(0)->removeRows(0, node.children.size());

    processes.erase(node.process.pid);
}

TreeNode getTree(QStandardItem *rootItem,
                 const std::map<Pid_t, Process> &flatMap,
                 const std::map<Pid_t, std::vector<QStandardItem *>> &itemMapping) {
    if (flatMap.empty())
        return {};
    Process init;
    bool initSet = false;
    for (auto &pair : flatMap) {
        if (!initSet || pair.second.pid < init.pid) {
            initSet = true;
            init = pair.second;
        }
    }
    return createNodeRecursive(rootItem, init, flatMap, itemMapping);
}

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
    QStandardItem *itemsRoot = model.invisibleRootItem();

    TreeNode tree = getTree(itemsRoot, processes, itemMapping);

    std::vector<Pid_t> del;
    for (auto &existingProcess : processes) {
        auto it = p.find(existingProcess.first);
        if (it == p.end() || it->second.threads.at(0).starttime != existingProcess.second.threads.at(0).starttime) {
            del.emplace_back(existingProcess.first);
        }
    }

    for (auto pid : del) {
        if (processes.find(pid) == processes.end())
            continue;
        TreeNode pr;
        assert(findNodeRecursive(tree, pid, pr));
        eraseNodeRecursive(pr, processes, model);
        pr.parent->removeRow(pr.items.at(0)->row());
    }

    del.clear();

    std::vector<Pid_t> newProcesses;

    for (auto &pair : p) {
        if (processes.find(pair.first) == processes.end()) {
            newProcesses.emplace_back(pair.first);
        }
    }

    while (!newProcesses.empty()) {
        for (auto &pid : newProcesses) {
            auto proc = p.at(pid);
            auto ppid = proc.threads.at(0).ppid;

            QList<QStandardItem *> l = getRow(p, proc.threads.at(0));
            if (ppid > 0) {
                if (itemMapping.find(ppid) != itemMapping.end()) {
                    itemMapping.at(ppid).at(0)->appendRow(l);
                } else {
                    continue;
                }
            } else {
                itemsRoot->appendRow(l);
            }

            for (auto *pntr : l)
                itemMapping[pid].emplace_back(pntr);

            del.emplace_back(pid);
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
