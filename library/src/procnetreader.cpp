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

#include "procnetreader.hpp"

#include <stdexcept>
#include <sstream>

#include "procpath.hpp"
#include "fileio.hpp"
#include "stringutil.hpp"
#include "strformat.hpp"

struct ProcNetEntry {
    std::string localAddress;
    std::string remoteAddress;
    std::string state;
    Inode_t inode{};
};

static std::vector<ProcNetEntry> readEntries(const std::string &contents) {
    std::vector<ProcNetEntry> ret;
    auto lines = StringUtil::readLines(contents);
    for (int i = 1; i < lines.size(); i++) {
        auto line = lines.at(i);
        line = StringUtil::removeSurroundingWhiteSpace(line);
        line = StringUtil::removeExtraWhitespace(line);
        auto split = StringUtil::splitString(line, " ");
        ProcNetEntry entry;
        entry.localAddress = split.at(1);
        entry.remoteAddress = split.at(2);
        entry.state = split.at(3);
        entry.inode = stringToInode(split.at(9));
        ret.emplace_back(entry);
    }
    return ret;
}

static int convertHexToDecmial(const std::string &hex) {
    int ret;
    std::stringstream ss;
    ss << std::hex << hex;
    ss >> ret;
    return ret;
}

static std::string convertHexAddress(const std::string &hex) {
    std::vector<std::string> hexParts;
    for (int i = 0; i < hex.size(); i += 2) {
        hexParts.emplace(hexParts.begin(), hex.substr(i, 2));
    }

    std::string ret;
    for (int i = 0; i < hexParts.size() - 1; i++) {
        ret += std::to_string(convertHexToDecmial(hexParts.at(i))) + ".";
    }
    ret += std::to_string(convertHexToDecmial(hexParts.at(hexParts.size() - 1)));
    return ret;
}

static int convertHexPort(const std::string &hex) {
    return convertHexToDecmial(hex);
}

static IpEndpoint getEndpoint(const std::string &address) {
    auto split = StringUtil::splitString(address, ":");
    return {convertHexAddress(split.at(0)), convertHexPort(split.at(1))};
}

//https://unix.stackexchange.com/a/470527
enum TCPStates : int {
    TCP_ESTABLISHED = 1,
    TCP_SYN_SENT,
    TCP_SYN_RECV,
    TCP_FIN_WAIT1,
    TCP_FIN_WAIT2,
    TCP_TIME_WAIT,
    TCP_CLOSE,
    TCP_CLOSE_WAIT,
    TCP_LAST_ACK,
    TCP_LISTEN,
    TCP_CLOSING,
    TCP_NEW_SYN_RECV
};

enum UDPStates : int {
    UDP_OPEN = 1,
    UDP_CLOSED = TCPStates::TCP_CLOSE // https://github.com/torvalds/linux/blob/a5c95ca18a98d742d0a4a04063c32556b5b66378/net/ipv4/udp.c#L1975
};

static std::string getStateName(SocketProtocol protocol, std::string state) {
    auto value = std::stoi(state);
    if (protocol == UDP) {
        switch (value) {
            case UDP_OPEN:
                return "UDP_OPEN";
            case UDP_CLOSED:
                return "UDP_CLOSED";
            default:
                return "ERR";
        }
    } else {
        switch (value) {
            case TCP_ESTABLISHED:
                return "TCP_ESTABLISHED";
            case TCP_SYN_SENT:
                return "TCP_SYN_SENT";
            case TCP_SYN_RECV:
                return "TCP_SYN_RECV";
            case TCP_FIN_WAIT1:
                return "TCP_FIN_WAIT1";
            case TCP_FIN_WAIT2:
                return "TCP_FIN_WAIT2";
            case TCP_TIME_WAIT:
                return "TCP_TIME_WAIT";
            case TCP_CLOSE:
                return "TCP_CLOSE";
            case TCP_CLOSE_WAIT:
                return "TCP_CLOSE_WAIT";
            case TCP_LAST_ACK:
                return "TCP_LAST_ACK";
            case TCP_LISTEN:
                return "TCP_LISTEN";
            case TCP_CLOSING:
                return "TCP_CLOSING";
            case TCP_NEW_SYN_RECV:
                return "TCP_NEW_SYN_RECV";
            default:
                return "ERR";
        }
    }
}

namespace ProcNetReader {
    std::map<Inode_t, Socket> getNetworkState() {
        std::map<Inode_t, Socket> ret;

        auto entries = readEntries(FileIO::readText(ProcPath::getProcNetTcp()));
        for (auto &entry: entries) {
            Socket socket;
            socket.protocol = TCP;
            socket.localEndpoint = getEndpoint(entry.localAddress);
            socket.remoteEndpoint = getEndpoint(entry.remoteAddress);
            socket.state = getStateName(TCP, entry.state);
            socket.inode = entry.inode;
            ret.insert(std::pair<Inode_t, Socket>(entry.inode, socket));
        }

        entries = readEntries(FileIO::readText(ProcPath::getProcNetUdp()));
        for (auto &entry: entries) {
            Socket socket;
            socket.protocol = UDP;
            socket.localEndpoint = getEndpoint(entry.localAddress);
            socket.remoteEndpoint = getEndpoint(entry.remoteAddress);
            socket.state = getStateName(UDP, entry.state);
            socket.inode = entry.inode;
            ret.insert(std::pair<Inode_t, Socket>(entry.inode, socket));
        }

        return ret;
    }
}
