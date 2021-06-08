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

#include "os/procreader.hpp"

#include <map>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <string>

#include <sys/stat.h>

#include "os/procpath.hpp"
#include "os/syscalls.hpp"

bool isAsciiNumber(char c) {
    return c >= '0' && c <= '9';
}

bool isAsciiNumber(const std::string &s) {
    for (auto c: s)
        if (!isAsciiNumber(c))
            return false;
    return true;
}

Mem_t getBytesFromKilobyte(const std::string &mem) {
    std::string tmp;
    tmp = mem.substr(0, mem.find(' '));
    return stringToMem(tmp) * 1000;
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

std::vector<std::string> splitString(const std::string &str, const std::string &delimiter) {
    std::vector<std::string> ret;
    auto s = str;
    auto pos = str.find(delimiter);
    while (pos != std::string::npos) {
        if (pos != 0) {
            ret.emplace_back(s.substr(0, pos));
        }
        s.erase(0, pos + delimiter.size());
        pos = s.find(delimiter);
    }
    return ret;
}

std::string getRealPath(const std::string &symLinkPath) {
    auto *p = realpath(symLinkPath.c_str(), NULL);
    if (p == NULL)
        throw std::runtime_error("Failed to get real path for " + symLinkPath);
    return p;
}

std::map<std::string, std::string> parseProcStr(const std::string &str) {
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

void parseProcCmdline(Process &proc) {
    try {
        proc.commandLine = readText(ProcPath::getProcessCommandLineFile(proc.pid));
    } catch (const std::exception &e) {} //Assume permissions error
}

void parseProcEnviron(Process &proc) {
    try {
        proc.environ = readText(ProcPath::getProcessEnvironFile(proc.pid));
    } catch (const std::exception &e) {} //Assume permissions error
}

void parseProcIo(Process &proc) {
    try {
        const auto d = parseProcStr(readText(ProcPath::getProcessIoFile(proc.pid)));
        proc.rchar = std::stoul(d.at("rchar"));
        proc.wchar = std::stoul(d.at("wchar"));
        proc.syscr = std::stoul(d.at("syscr"));
        proc.syscw = std::stoul(d.at("syscw"));
        proc.read_bytes = std::stoul(d.at("read_bytes"));
        proc.write_bytes = std::stoul(d.at("write_bytes"));
        proc.cancelled_write_bytes = std::stoul(d.at("cancelled_write_bytes"));
    } catch (const std::exception &e) {} //Assume permissions error
}

void parseProcRoot(Process &proc) {
    try {
        proc.rootDirectory = getRealPath(ProcPath::getProcessRootDirectory(proc.pid));
    } catch (const std::exception &e) {} //Assume permissions error
}

void parseProcFd(Process &proc) {
    try {
        const auto p = ProcPath::getProcessFdDirectory(proc.pid);
        for (auto &f : std::filesystem::directory_iterator(p)) {
            proc.openFiles.emplace_back(getRealPath(f.path()));
        }
    } catch (const std::exception &e) {} //Assume permissions error
}

void parseProcExe(Process &proc) {
    try {
        proc.executablePath = getRealPath(ProcPath::getProcessRootDirectory(proc.pid));
    } catch (const std::exception &e) {} //Assume permissions error
}

void parseThreadStat(Thread &thread) {
    try {
        auto text = readText(ProcPath::getThreadStatFile(thread.pid, thread.tid));
        auto firstBracket = text.find_first_of('(');
        auto lastBracket = text.find_last_of(')');

        thread.comm = text.substr(firstBracket + 1, lastBracket - firstBracket - 1);

        text.erase(0, lastBracket + 1);

        auto split = splitString(text, " ");

        thread.state = split.at(0)[0];
        thread.ppid = std::stoi(split.at(1));
        thread.pgrp = std::stoi(split.at(2));
        thread.session = std::stoi(split.at(3));
        thread.tty_nr = std::stoi(split.at(4));
        thread.tpgid = std::stoi(split.at(5));
        thread.flags = std::stoi(split.at(6));
        thread.minflt = std::stoul(split.at(7));
        thread.cminflt = std::stoul(split.at(8));
        thread.majflt = std::stoul(split.at(9));
        thread.cmajflt = std::stoul(split.at(10));
        thread.utime = std::stoul(split.at(11));
        thread.stime = std::stoul(split.at(12));
        thread.cutime = std::stol(split.at(13));
        thread.cstime = std::stol(split.at(14));
        thread.priority = std::stol(split.at(15));
        thread.nice = std::stol(split.at(16));
        thread.num_threads = std::stol(split.at(17));
        thread.itrealvalue = std::stol(split.at(18));
        thread.starttime = std::stoull(split.at(19));
        thread.vsize = std::stoul(split.at(20));
        thread.rss = std::stol(split.at(21));
        thread.rsslim = std::stoul(split.at(22));
        thread.startcode = std::stoul(split.at(23));
        thread.endcode = std::stoul(split.at(24));
        thread.startstack = std::stoul(split.at(25));
        thread.kstkesp = std::stoul(split.at(26));
        thread.kstkeip = std::stoul(split.at(27));
        thread.signal = std::stoul(split.at(28));
        thread.blocked = std::stoul(split.at(29));
        thread.sigignore = std::stoul(split.at(30));
        thread.sigcatch = std::stoul(split.at(31));
        thread.wchan = std::stoul(split.at(32));
        thread.nswap = std::stoul(split.at(33));
        thread.cnswap = std::stoul(split.at(34));

        if (split.size() < 36)
            return;

        thread.exit_signal = std::stoi(split.at(35));

        if (split.size() < 37)
            return;

        thread.processor = std::stoi(split.at(36));

        if (split.size() < 38)
            return;

        thread.rt_priority = std::stoul(split.at(37));

        if (split.size() < 39)
            return;

        thread.policy = std::stoul(split.at(38));

        if (split.size() < 40)
            return;

        thread.delayacct_blkio_ticks = std::stoull(split.at(39));

        if (split.size() < 41)
            return;

        thread.guest_time = std::stoul(split.at(40));

        if (split.size() < 42)
            return;

        thread.cguest_time = std::stol(split.at(41));

        if (split.size() < 43)
            return;

        thread.start_data = std::stoul(split.at(42));

        if (split.size() < 44)
            return;

        thread.end_data = std::stoul(split.at(43));

        if (split.size() < 45)
            return;

        thread.start_brk = std::stoul(split.at(44));

        if (split.size() < 46)
            return;

        thread.arg_start = std::stoul(split.at(45));

        if (split.size() < 47)
            return;

        thread.arg_end = std::stoul(split.at(46));

        if (split.size() < 48)
            return;

        thread.env_start = std::stoul(split.at(47));

        if (split.size() < 49)
            return;

        thread.env_end = std::stoul(split.at(48));

        if (split.size() < 50)
            return;

        thread.exit_code = std::stoi(split.at(49));

    } catch (const std::exception &e) {} //Assume permissions error
}

void parseThreadStatm(Thread &thread) {
    try {
        auto split = splitString(readText(ProcPath::getThreadStatMFile(thread.pid, thread.tid)), " ");
        thread.size = std::stoul(split.at(0));
        thread.resident = std::stoul(split.at(1));
        thread.shared = std::stoul(split.at(2));
        thread.text = std::stoul(split.at(3));
        thread.lib = std::stoul(split.at(4));
        thread.data = std::stoul(split.at(5));
        thread.dt = std::stoul(split.at(6));
    } catch (const std::exception &e) {} //Assume permissions error
}

void parseThreadCwd(Thread &thread) {
    try {
        thread.cwd = getRealPath(ProcPath::getThreadCwdFile(thread.pid, thread.tid));
    } catch (const std::exception &e) {} //Assume permissions error
}

Uid_t getFileOwnerUid(const std::filesystem::path &p) {
    try {
        struct stat s{};
        stat(p.c_str(), &s);
        return s.st_uid;
    } catch (const std::exception &e) {} //Assume permissions error
}

namespace ProcReader {
    bool isPID(const std::string &name) {
        for (auto &c : name) {
            if (!isAsciiNumber(c))
                return false;
        }
        return true;
    }

    Thread readThread(Process &p, Pid_t tid) {
        Thread t;

        t.pid = p.pid;
        t.uid = p.uid;
        t.tid = tid;

        parseThreadStat(t);
        parseThreadStatm(t);
        parseThreadCwd(t);

        return t;
    }

    Process readProcess(Pid_t pid) {
        Process p;
        p.pid = pid;
        p.uid = getFileOwnerUid(ProcPath::getProcessDirectory(pid));

        parseProcCmdline(p);
        parseProcEnviron(p);
        parseProcIo(p);
        parseProcRoot(p);
        parseProcFd(p);
        parseProcExe(p);

        std::string dir = ProcPath::getProcessTasksDirectory(pid);

        for (auto &d : std::filesystem::directory_iterator(dir)) {
            auto t = readThread(p, stringToPid(d.path().filename()));
            if (t.tid == p.pid)
                p.threads.emplace(p.threads.begin(), t);
            else
                p.threads.emplace_back(t);
        }

        return p;
    }

    Memory readMemory() {
        auto proc = parseProcStr(readText(ProcPath::getMemoryInfoFile()));

        Memory ret{};

        ret.total = getBytesFromKilobyte(proc.at("MemTotal"));
        ret.free = getBytesFromKilobyte(proc.at("MemFree"));
        ret.available = getBytesFromKilobyte(proc.at("MemAvailable"));

        return ret;
    }
}