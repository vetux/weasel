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

static std::string getStateName(SocketProtocol protocol, std::string state) {
    return "ERR"; //TODO: Convert state name
}

namespace ProcNetReader {
    std::map<Inode_t, Socket> getNetworkState() {
        std::map<Inode_t, Socket> ret;

        auto entries = readEntries(FileIO::readText(ProcPath::getProcNetTcp()));
        for (auto &entry : entries) {
            Socket socket;
            socket.protocol = TCP;
            socket.localEndpoint = getEndpoint(entry.localAddress);
            socket.remoteEndpoint = getEndpoint(entry.remoteAddress);
            socket.state = getStateName(TCP, entry.state);
            socket.inode = entry.inode;
            ret.insert(std::pair<Inode_t, Socket>(entry.inode, socket));
        }

        entries = readEntries(FileIO::readText(ProcPath::getProcNetUdp()));
        for (auto &entry : entries) {
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
