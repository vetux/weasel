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

#ifndef WEASEL_SOCKET_HPP
#define WEASEL_SOCKET_HPP

#include <string>

#include "weasel/types.hpp"

enum SocketProtocol {
    UDP,
    TCP
};

struct IpEndpoint {
    bool operator==(const IpEndpoint &other) const {
        return address == other.address
               && port == other.port;
    }

    std::string toString() const {
        return address + ":" + std::to_string(port);
    }

    std::string address;
    int port;
};

struct Socket {
    bool operator==(const Socket &other) const {
        return inode == other.inode
               && protocol == other.protocol
               && localEndpoint == other.localEndpoint
               && remoteEndpoint == other.remoteEndpoint;
    }

    bool operator!=(const Socket &other) const {
        return !(*this == other);
    }

    Inode_t inode;
    SocketProtocol protocol;
    IpEndpoint localEndpoint;
    IpEndpoint remoteEndpoint;
    std::string state;
};

#endif //WEASEL_SOCKET_HPP
