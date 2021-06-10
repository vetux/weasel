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

std::vector<Pid_t> getPidRecursive(TreeNode &tree) {
    std::vector<Pid_t> ret;
    ret.emplace_back(tree.process.pid);
    for (auto *child : tree.children) {
        auto tmp = getPidRecursive(*child);
        for (auto t : tmp)
            ret.emplace_back(t);
    }
    return ret;
}

bool operator==(const TreeNode &lhs, const TreeNode &rhs) {
    return lhs.process.pid == rhs.process.pid
           && lhs.process.threads.at(0).starttime == rhs.process.threads.at(0).starttime;
}

QList<QStandardItem *> getRow(const Process &proc) {
    QList<QStandardItem *> ret;
    ret.append(new QStandardItem(QString("%0").arg(proc.threads.at(0).comm.c_str())));
    ret.append(new QStandardItem(QString("%0").arg(proc.threads.at(0).pid)));
    ret.append(new QStandardItem(QString("%0").arg(Users::getUserName(proc.threads.at(0).uid).c_str())));
    ret.append(new QStandardItem(QString("%0").arg(proc.commandLine.c_str())));
    return ret;
}

bool findNodeRecursive(TreeNode &tree, Pid_t pid, TreeNode *&output) {
    if (tree.process.pid == pid) {
        output = &tree;
        return true;
    }

    for (auto *child : tree.children) {
        if (findNodeRecursive(*child, pid, output))
            return true;
    }

    return false;
}

TreeNode *createNode(TreeNode &parent, const Process &process) {
    auto *ret = new TreeNode();
    ret->parent = &parent;
    ret->process = process;
    ret->items = getRow(process);
    parent.items.at(0)->appendRow(ret->items);
    parent.children.emplace_back(ret);
    return ret;
}

void eraseNodeRecursive(TreeNode &tree, TreeNode *node) {
    for (auto *child : node->children) {
        eraseNodeRecursive(tree, child);
    }
    node->items.at(0)->removeRows(0, node->children.size());
    node->parent->items.at(0)->removeRow(node->items.at(0)->row());
    auto it = std::find(node->parent->children.begin(), node->parent->children.end(), node);
    assert(it != node->parent->children.end());
    node->parent->children.erase(it);
    delete node;
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

    tree.items.append(model.invisibleRootItem());
}

ProcessTreeWidget::~ProcessTreeWidget() {

}

void ProcessTreeWidget::setProcesses(const std::map<Pid_t, Process> &p) {
    std::vector<Pid_t> deadProcesses;

    for (auto &pair : p) {
        TreeNode *n;
        if (findNodeRecursive(tree, pair.first, n)) {
            if (n->process.threads.at(0).starttime != pair.second.threads.at(0).starttime) {
                deadProcesses.emplace_back(pair.first);
            }
        } else {
            deadProcesses.emplace_back(pair.first);
        }
    }

    auto pids = getPidRecursive(tree);
    for (auto pid : pids) {
        if (p.find(pid) == p.end()) {
            deadProcesses.emplace_back(pid);
        }
    }

    for (auto deadPid : deadProcesses) {
        if (deadPid == 0)
            continue;
        TreeNode *n;
        if (findNodeRecursive(tree, deadPid, n)) {
            eraseNodeRecursive(tree, n);
        }
    }

    std::map<Pid_t, Pid_t> newProcesses;

    for (auto &pair : p) {
        TreeNode *n;
        if (!findNodeRecursive(tree, pair.first, n)) {
            newProcesses[pair.first] = pair.second.threads.at(0).ppid;
        }
    }

    std::vector<Pid_t> del;
    while (!newProcesses.empty()) {
        for (auto pair : newProcesses) {
            if (pair.second == 0) {
                createNode(tree, p.at(pair.first));
                del.emplace_back(pair.first);
            } else {
                TreeNode *parent;
                if (findNodeRecursive(tree, pair.second, parent)) {
                    createNode(*parent, p.at(pair.first));
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
