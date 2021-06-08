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

ProcessTreeWidget::ProcessTreeWidget(QWidget *parent) : QWidget(parent) {
    setLayout(new QVBoxLayout());
    treeView = new QTreeView();

    model.setColumnCount(4);

    QStandardItem *parentItem = model.invisibleRootItem();

    for (int i = 0; i < 4; ++i) {
        auto *item = new QStandardItem(QString("item %0").arg(i));
        QList<QStandardItem *> l;
        l.append(item);
        for (int y = 0; y < 4; y++) {
            l.append(new QStandardItem(QString("item %0 col %1").arg(i).arg(y)));
        }
        parentItem->appendRow(l);
        parentItem = item;
    }

    treeView->setModel(&model);

    connect(treeView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(doubleCLicked(const QModelIndex &)));

    connect(treeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(clicked()));

    layout()->addWidget(treeView);
}

ProcessTreeWidget::~ProcessTreeWidget() {

}

void ProcessTreeWidget::setProcesses(const std::map<Pid_t, Process> &processes) {
    model.clear();

    QStandardItem *parentItem = model.invisibleRootItem();

    for (auto &p : processes) {
        auto *item = new QStandardItem(QString("%0").arg(p.first));
        QList<QStandardItem *> l;
        l.append(item);
        l.append(new QStandardItem(QString("%0").arg(p.second.threads.at(0).comm.c_str())));
        parentItem->appendRow(l);
        parentItem = item;
    }
}

void ProcessTreeWidget::clicked() {

}

void ProcessTreeWidget::doubleCLicked(const QModelIndex &index) {

}
