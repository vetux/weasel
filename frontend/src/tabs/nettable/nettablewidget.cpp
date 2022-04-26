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

    tableWidget->setColumnCount(5);
    tableWidget->setHorizontalHeaderLabels({"Local", "Remote", "Protocol", "Pid", "Executable"});
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
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
                                           new QLabel(std::to_string(p.first).c_str()));
                tableWidget->setCellWidget(tableWidget->rowCount() - 1, 4,
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

        contextMenu->addAction(new Action("Copy To Clipboard", COPY));
        contextMenu->addSeparator();
        contextMenu->addAction(new Action("Go To Process", VIEW_PROCESS));
        contextMenu->addAction(new Action("Terminate Process", TERMINATE));

        connect(contextMenu,
                &QMenu::triggered,
                [this, index, item](QAction *action) {
                    if (TypeCheck::checkCast<QAction, Action>(*action)) {
                        auto &pa = dynamic_cast<Action &>(*action);
                        switch (pa.type) {
                            case TERMINATE:
                                emit terminateProcess(processes.at(index.row()));
                                break;
                            case VIEW_PROCESS:
                                emit viewProcess(processes.at(index.row()));
                                break;
                            case COPY:
                                QGuiApplication::clipboard()->setText(item->text());
                                break;
                        }
                    }
                }
        );

        contextMenu->exec(tableWidget->viewport()->mapToGlobal(pos));

        delete contextMenu;
    }
}
