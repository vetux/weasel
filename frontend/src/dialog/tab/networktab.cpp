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

#include "dialog/tab/networktab.hpp"

#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>

static QString getProtocolString(SocketProtocol protocol) {
    switch (protocol) {
        case UDP:
            return "UDP";
        case TCP:
            return "TCP";
        default:
            assert(false);
    }
}

NetworkTab::NetworkTab(QWidget *parent) {
    setLayout(new QVBoxLayout());
    socketsTree = new QTreeView(this);
    socketsModel = new QStandardItemModel();
    socketsTree->setModel(socketsModel);
    layout()->addWidget(new QLabel("Connections"));
    layout()->addWidget(socketsTree);

    socketsTree->setIndentation(0);

    socketsModel->setColumnCount(1);
    socketsTree->header()->hide();
    socketsModel->insertRow(0, new QStandardItem("No Connections"));
}

void NetworkTab::setSocket(const Socket &socket) {
    if (rowMapping.find(socket.inode) != rowMapping.end())
        return;

    auto row = 0;

    if (rowMapping.empty()) {
        socketsModel->clear();
        socketsModel->setColumnCount(4);
        socketsTree->header()->show();
        socketsTree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
        socketsTree->header()->setSectionResizeMode(1, QHeaderView::Stretch);
        socketsTree->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
        socketsTree->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
        socketsTree->header()->setStretchLastSection(false);
        socketsModel->setHorizontalHeaderLabels({"Local Endpoint", "Remote Endpoint", "Protocol", "State"});
    } else {
        row = socketsModel->rowCount();
    }

    socketsModel->insertRow(row,
                            {
                                    new QStandardItem(socket.localEndpoint.toString().c_str()),
                                    new QStandardItem(socket.remoteEndpoint.toString().c_str()),
                                    new QStandardItem(getProtocolString(socket.protocol)),
                                    new QStandardItem(socket.state.c_str())
                            });
    rowMapping[socket.inode] = row;
}

void NetworkTab::removeSocket(const Socket &socket) {
    bool startDecrement = false;
    for (auto &pair: rowMapping) {
        if (startDecrement) {
            pair.second--;
        }
        if (pair.first == socket.inode) {
            startDecrement = true;
        }
    }
    if (rowMapping.find(socket.inode) != rowMapping.end()) {
        socketsModel->removeRow(rowMapping.at(socket.inode));
        rowMapping.erase(socket.inode);
    }
    if (rowMapping.empty()) {
        socketsModel->clear();
        socketsModel->setColumnCount(1);
        socketsTree->header()->hide();
        socketsModel->insertRow(0, new QStandardItem("No Connections"));
    }
}

void NetworkTab::clearSockets() {
    socketsModel->clear();
}

void NetworkTab::setReadBytes(const std::string &bytes) {

}

void NetworkTab::setWriteBytes(const std::string &bytes) {

}

void NetworkTab::setReadRate(const std::string &rate) {

}

void NetworkTab::setWriteRate(const std::string &rate) {

}