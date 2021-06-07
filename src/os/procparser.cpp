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

#include "os/procparser.hpp"

#include <map>
#include <fstream>
#include <cstring>

#include <sys/resource.h>

#include "os/procpath.hpp"
#include "os/syscalls.hpp"

bool isAsciiNumber(char c) {
    return c >= '0' && c <= '9';
}

std::string removeDoubleWhiteSpace(const std::string &str) {
    auto ret = str;
    auto space = ret.find("  ");
    while (space != -1) {
        ret.replace(space, 2, " ");
        space = ret.find("  ");
    }
    return ret;
}

std::string removeSurroundingWhiteSpace(const std::string &str) {
    auto ret = str;
    auto space = ret.find(' ');
    while (space == 0) {
        ret.erase(space, 1);
        space = ret.find(' ');
    }
    space = ret.find(' ');
    while (!ret.empty() && space == ret.size() - 1) {
        ret.erase(space, 1);
        space = ret.find(' ');
    }
    return ret;
}

std::string removeTabs(const std::string &str) {
    auto ret = str;
    auto tab = ret.find('\t');
    while (tab != -1) {
        ret.replace(tab, 1, " ");
        tab = ret.find('\t');
    }
    return ret;
}

std::map<std::string, std::string> parseProcFile(const std::string &str) {
    std::istringstream f(str);

    std::map<std::string, std::string> ret;

    std::string line;
    while (std::getline(f, line)) {
        auto dot = line.find(':');
        if (dot == std::string::npos)
            throw std::runtime_error("Invalid proc file format");
        std::string name = line.substr(0, dot);
        std::string value = line.substr(dot + 1);
        value = removeTabs(value);
        value = removeDoubleWhiteSpace(value);
        value = removeSurroundingWhiteSpace(value);
        if (ret.find(name) != ret.end())
            throw std::runtime_error("Invalid proc file double key");
        ret[name] = value;
    }

    return ret;
}

std::string readText(const std::string &filePath) {
    std::string ret;
    std::ifstream stream;

    stream.open(filePath);

    if (stream.fail()) {
        throw std::runtime_error("Failed to read text at " + filePath + " error: " + strerror(errno));
    }

    std::string tmp;
    while (std::getline(stream, tmp)) {
        ret += tmp + "\n";
    }

    return ret;
}

Uid_t getUID(const std::string &uid) {
    std::string t = uid.substr(0, uid.find(' '));
    return stringToUid(t);
}

Mem_t getBytesFromKilobyte(const std::string &mem) {
    std::string tmp;
    tmp = mem.substr(0, mem.find(' '));
    return stringToMem(tmp) * 1000;
}

std::string readCommand(Pid_t PID) {
    std::ifstream stream;

    std::string filePath = ProcPath::getCommandLineFile(PID);
    stream.open(filePath);

    if (stream.fail()) {
        throw std::runtime_error("Failed to read text at " + filePath + " error: " + strerror(errno));
    }

    std::string ret;
    stream >> ret;
    return ret;
}

SchedulingPolicy convertPolicy(uint32_t policy) {
    switch (policy) {
        case SCHED_OTHER:
            return OTHER;
        case SCHED_BATCH:
            return BATCH;
        case SCHED_IDLE:
            return IDLE;
        case SCHED_FIFO:
            return FIFO;
        case SCHED_RR:
            return RR;
        case SCHED_DEADLINE:
            return DEADLINE;
        default:
            throw std::runtime_error("Invalid policy value");
    }
}

namespace ProcParser {
    bool isPID(const std::string &name) {
        for (auto &c : name) {
            if (!isAsciiNumber(c))
                return false;
        }
        return true;
    }

    Thread parseThread(const std::filesystem::path &statusFile) {
        auto proc = parseProcFile(readText(statusFile));

        Thread ret;

        ret.TID = stringToPid(proc.at("Pid"));
        ret.PID = stringToPid(proc.at("Tgid"));
        ret.UID = getUID(proc.at("Uid"));

        ret.name = proc.at("Name");

        errno = 0;
        ret.priority = getpriority(PRIO_PROCESS, ret.TID);
        auto err = errno;
        if (ret.priority == -1 && err != 0) {
            throw std::runtime_error("Failed to get priority");
        }

        sched_attr attr{};
        sched_getattr(ret.TID, &attr, sizeof(struct sched_attr), 0);

        ret.policy = convertPolicy(attr.sched_policy);

        //TODO:Implement memory parsing
        ret.memVirt = 0;
        ret.memRes = 0;
        ret.memShared = 0;

        return ret;
    }

    Process parseProcess(const std::filesystem::path &statusFile) {
        auto proc = parseProcFile(readText(statusFile));
        Process ret;

        ret.PID = stringToPid(proc.at("Pid"));
        ret.UID = getUID(proc.at("Uid"));

        ret.name = proc.at("Name");
        ret.command = readCommand(ret.PID);

        errno = 0;
        ret.priority = getpriority(PRIO_PROCESS, ret.PID);
        auto err = errno;
        if (ret.priority == -1 && err != 0) {
            throw std::runtime_error("Failed to get priority");
        }

        //TODO:Implement memory parsing
        ret.memVirt = 0;
        ret.memRes = 0;
        ret.memShared = 0;

        ret.parentPID = stringToPid(proc.at("PPid"));

        std::vector<std::filesystem::directory_entry> entries;
        for (auto &fl : std::filesystem::directory_iterator(ProcPath::getProcessTasksDirectory(ret.PID))) {
            if (isPID(fl.path().filename())) {
                entries.emplace_back(fl);
            }
        }

        for (auto &e : entries) {
            ret.threads.emplace_back(
                    parseThread(ProcPath::getThreadStatusFile(ret.PID, stringToPid(e.path().filename().string())))
            );
        }

        return ret;
    }

    Memory parseMemory(const std::filesystem::path &memFile) {
        auto proc = parseProcFile(readText(memFile));

        Memory ret{};

        ret.total = getBytesFromKilobyte(proc.at("MemTotal"));
        ret.free = getBytesFromKilobyte(proc.at("MemFree"));
        ret.available = getBytesFromKilobyte(proc.at("MemAvailable"));

        return ret;
    }
}