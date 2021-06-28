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

#include "system/procnetreader.hpp"
#include "system/procpath.hpp"
#include "system/fileio.hpp"
#include "system/stringutil.hpp"

#include <stdexcept>

struct ProcNetEntry {
    std::string localAddress;
    std::string remoteAddress;
    std::string state;
    std::string inode;
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
        entry.inode = split.at(9);
        ret.emplace_back(entry);
    }
    return ret;
}

static std::string convertHexAddress(const std::string &hex) {
    return hex; //TODO: Convert hex address
}

static int convertHexPort(const std::string &hex) {
    return 0; //TODO: Convert hex port
}

static IpEndpoint getEndpoint(const std::string &address) {
    auto split = StringUtil::splitString(address, ":");
    return {convertHexAddress(split.at(0)), convertHexPort(split.at(1))};
}

static std::string getStateName(SocketProtocol protocol, std::string state) {
    return "ERR"; //TODO: Convert state name
}

namespace ProcNetReader {
    Socket getSocketFromInode(const std::string &inode) {
        auto entries = readEntries(FileIO::readText(ProcPath::getProcNetTcp()));
        for (auto &entry : entries) {
            if (entry.inode == inode) {
                Socket ret;
                ret.protocol = TCP;
                ret.localEndpoint = getEndpoint(entry.localAddress);
                ret.remoteEndpoint = getEndpoint(entry.remoteAddress);
                ret.state = getStateName(TCP, entry.state);
                return ret;
            }
        }

        entries = readEntries(FileIO::readText(ProcPath::getProcNetUdp()));
        for (auto &entry : entries) {
            if (entry.inode == inode) {
                Socket ret;
                ret.protocol = UDP;
                ret.localEndpoint = getEndpoint(entry.localAddress);
                ret.remoteEndpoint = getEndpoint(entry.remoteAddress);
                ret.state = getStateName(UDP, entry.state);
                return ret;
            }
        }

        throw std::runtime_error("Socket Inode not found.");
    }
}
