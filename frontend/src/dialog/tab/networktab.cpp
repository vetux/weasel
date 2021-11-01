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

static QString getFormattedConnectionString(const Socket &sock) {
    std::string prefix;
    if (sock.protocol == TCP)
        prefix = "TCP\t";
    else
        prefix = "UDP\t";
    return (prefix
            + sock.localEndpoint.address + ":" + std::to_string(sock.localEndpoint.port)
            + "\t->\t"
            + sock.remoteEndpoint.address + ":" +
            std::to_string(sock.remoteEndpoint.port)).c_str();
}

NetworkTab::NetworkTab(QWidget *parent) {
    setLayout(new QVBoxLayout());
    socketsList = new QListWidget(this);
    layout()->addWidget(socketsList);
}

void NetworkTab::setSockets(const std::vector<Socket> &sockets) {
    socketsList->clear();
    for (auto &socket: sockets) {
        socketsList->addItem(getFormattedConnectionString(socket));
    }
}

void NetworkTab::setReadBytes(const std::string& bytes) {

}

void NetworkTab::setWriteBytes(const std::string& bytes) {

}

void NetworkTab::setReadRate(const std::string& rate) {

}

void NetworkTab::setWriteRate(const std::string& rate) {

}