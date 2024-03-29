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

#include "tabs/nettable/nettablewidget.hpp"

#include <QVBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMenu>
#include <QClipboard>
#include <QGuiApplication>

#include "typecheck.hpp"

NetTableWidget::NetTableWidget() {
    tableWidget = new QTableWidget();

    tableWidget->setColumnCount(6);
    tableWidget->setHorizontalHeaderLabels({"Local", "Remote", "Protocol", "State", "Pid", "Executable"});
    tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode::Stretch);
    tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
    tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeMode::Stretch);
    tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeMode::Stretch);
    tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeMode::Stretch); // ResizeToContents causes the columns to be randomly resized correctly to the largest content width or resized to column header width when switching between tree tab and network tab.
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    tableWidget->verticalHeader()->hide();

    setLayout(new QVBoxLayout());

    layout()->addWidget(tableWidget);

    layout()->setMargin(0);

    connect(tableWidget,
            SIGNAL(customContextMenuRequested(const QPoint &)),
            this,
            SLOT(customContextMenu(const QPoint &)));
}

void NetTableWidget::onSnapshot(const Snapshot &snapshot) {
    if (!snapshot.deadSockets.empty() || !snapshot.spawnedSockets.empty() || tableWidget->rowCount() == 0) {
        tableWidget->setRowCount(0);
        processes.clear();
        for (auto &p: snapshot.processes) {
            for (auto &socket: p.second.sockets) {
                tableWidget->insertRow(tableWidget->rowCount());
                tableWidget->setCellWidget(tableWidget->rowCount() - 1, 0,
                                           new QLabel(socket.localEndpoint.toString().c_str()));
                tableWidget->setCellWidget(tableWidget->rowCount() - 1, 1,
                                           new QLabel(socket.remoteEndpoint.toString().c_str()));
                tableWidget->setCellWidget(tableWidget->rowCount() - 1, 2,
                                           new QLabel(socket.protocol == TCP ? "TCP" : "UDP"));
                tableWidget->setCellWidget(tableWidget->rowCount() - 1, 3,
                                           new QLabel(socket.state.c_str()));
                tableWidget->setCellWidget(tableWidget->rowCount() - 1, 4,
                                           new QLabel(std::to_string(p.first).c_str()));
                tableWidget->setCellWidget(tableWidget->rowCount() - 1, 5,
                                           new QLabel(p.second.executablePath.c_str()));
                processes.emplace_back(p.first);
            }
        }
    }
}

void NetTableWidget::customContextMenu(const QPoint &pos) {
    auto index = tableWidget->indexAt(pos);

    if (index.isValid()) {
        auto *item = dynamic_cast<QLabel *>(tableWidget->indexWidget(index));
        auto *contextMenu = new QMenu(tableWidget);

        auto pid = processes.at(index.row());
        auto itemText = item->text();

        contextMenu->addAction(new Action("Copy To Clipboard", COPY));
        contextMenu->addSeparator();
        contextMenu->addAction(new Action("Go To Process", VIEW_PROCESS));
        contextMenu->addAction(new Action("Terminate Process", TERMINATE));

        connect(contextMenu,
                &QMenu::triggered,
                [this, pid, itemText](QAction *action) {
                    if (TypeCheck::checkCast<QAction, Action>(*action)) {
                        if (std::find(processes.begin(), processes.end(), pid) == processes.end()){
                            return; //Process / Network connection does not exist anymore
                        }
                        auto &pa = dynamic_cast<Action &>(*action);
                        switch (pa.type) {
                            case TERMINATE:
                                emit terminateProcess(pid);
                                break;
                            case VIEW_PROCESS:
                                emit viewProcess(pid);
                                break;
                            case COPY:
                                QGuiApplication::clipboard()->setText(itemText);
                                break;
                        }
                    }
                }
        );

        contextMenu->exec(tableWidget->viewport()->mapToGlobal(pos));

        delete contextMenu;
    }
}
