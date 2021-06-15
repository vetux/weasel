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

#include "system/procreader.hpp"

#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

#include <cstdlib>
#include <cstring>

#include <sys/stat.h>

#include "system/procpath.hpp"
#include "system/syscalls.hpp"

inline bool isAsciiNumber(char c) {
    return c >= '0' && c <= '9';
}

Mem_t getBytesFromKilobyte(const std::string &mem) {
    auto str = mem.substr(0, mem.find(' '));
    return stringToMem(str) * 1000;
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

std::string readText(const std::string &filePath, const std::string &newLine = "\n") {
    std::string ret;
    std::ifstream stream;

    stream.open(filePath);

    if (stream.fail()) {
        throw std::runtime_error("Failed to read text at " + filePath + " error: " + strerror(errno));
    }

    std::string tmp;
    while (std::getline(stream, tmp)) {
        ret += tmp + newLine;
    }

    return ret;
}

std::vector<std::string> readLines(const std::string &str) {
    std::vector<std::string> ret;
    std::stringstream stream(str);

    std::string tmp;
    while (std::getline(stream, tmp)) {
        ret.emplace_back(tmp);
    }

    return ret;
}

void removeSurroundingWhiteSpace(std::string &str) {
    //Preceding
    auto space = str.find_first_of(' ');
    while (space == 0) {
        str.erase(space, 1);
        space = str.find_first_of(' ');
    }

    //Trailing
    space = str.find_last_of(' ');
    while (!str.empty() && space == str.size() - 1) {
        str.erase(space, 1);
        space = str.find_last_of(' ');
    }
}

void replace(std::string &str, const std::string &v, const std::string &r) {
    size_t pos = str.find(v);
    while (pos != std::string::npos) {
        str.replace(pos, v.size(), r);
        pos = str.find(v);
    }
}

std::vector<std::string> splitString(const std::string &str, const std::string &delimiter) {
    if (str.empty())
        return {};

    std::vector<std::string> ret;

    size_t startPos = 0;
    size_t pos = str.find(delimiter);
    while (pos != std::string::npos) {
        if (pos != 0) {
            ret.emplace_back(str.substr(startPos, pos - startPos));
        }

        if (pos == str.size() - 1)
            break;

        startPos = pos + 1;
        pos = str.find(delimiter, pos + 1);
    }

    ret.emplace_back(str.substr(startPos, pos));

    return ret;
}

std::string getRealPath(const std::string &symLinkPath) {
    auto *p = realpath(symLinkPath.c_str(), NULL);
    if (p == NULL)
        throw std::runtime_error("Failed to get real path for " + symLinkPath);
    return p;
}

std::map<std::string, std::string> parseProcStr(const std::string &str) {
    std::map<std::string, std::string> ret;

    std::istringstream f(str);
    std::string line;
    while (std::getline(f, line)) {
        auto dot = line.find(':');
        if (dot == std::string::npos)
            throw std::runtime_error("Invalid proc file format");

        std::string name = line.substr(0, dot);
        std::string value = line.substr(dot + 1);

        replace(value, "\t", " ");
        replace(value, "  ", " ");
        removeSurroundingWhiteSpace(value);

        ret[name] = value;
    }

    return ret;
}

void parseProcCmdline(Process &proc) {
    try {
        proc.commandLine = readText(ProcPath::getProcessCommandLineFile(proc.pid), " ");
        replace(proc.commandLine, std::string("\0", 1), " ");
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

SystemStatus::Core readCore(const std::vector<std::string> &line) {
    SystemStatus::Core ret{};
    ret.user = std::stoul(line.at(1));
    ret.nice = std::stoul(line.at(2));
    ret.system = std::stoul(line.at(3));
    ret.idle = std::stoul(line.at(4));

    if (line.size() < 6)
        return ret;

    ret.iowait = std::stoul(line.at(5));

    if (line.size() < 7)
        return ret;

    ret.irq = std::stoul(line.at(6));

    if (line.size() < 8)
        return ret;

    ret.softirq = std::stoul(line.at(7));

    if (line.size() < 9)
        return ret;

    ret.steal = std::stoul(line.at(8));

    if (line.size() < 10)
        return ret;

    ret.guest = std::stoul(line.at(9));

    if (line.size() < 11)
        return ret;

    ret.guest_nice = std::stoul(line.at(10));

    return ret;
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

    SystemStatus readSystemStatus() {
        auto statContents = readText(ProcPath::getProcStatFile());
        replace(statContents, "  ", " "); //Replace double whitespace

        auto stat = readLines(statContents);
        auto memInfo = parseProcStr(readText(ProcPath::getMemoryInfoFile()));

        SystemStatus ret{};

        for (auto &line : stat) {
            auto lineSplit = splitString(line, " ");
            if (lineSplit.at(0).rfind("cpu", 0) == 0) {
                if (lineSplit.at(0) == "cpu") {
                    ret.cpu = readCore(lineSplit);
                } else {
                    ret.cores.emplace_back(readCore(lineSplit));
                }
            } else if (lineSplit.at(0) == "page") {
                ret.pageIn = std::stol(lineSplit.at(1));
                ret.pageOut = std::stol(lineSplit.at(2));
            } else if (lineSplit.at(0) == "swap") {
                ret.swapIn = std::stol(lineSplit.at(1));
                ret.swapOut = std::stol(lineSplit.at(2));
            } else if (lineSplit.at(0) == "intr") {
                ret.interrupts = std::stol(lineSplit.at(1));
            } else if (lineSplit.at(0) == "ctxt") {
                ret.contextSwitches = std::stol(lineSplit.at(1));
            } else if (lineSplit.at(0) == "btime") {
                ret.btime = std::stoul(lineSplit.at(1));
            } else if (lineSplit.at(0) == "processes") {
                ret.processes = std::stol(lineSplit.at(1));
            } else if (lineSplit.at(0) == "procs_running") {
                ret.processesRunning = std::stol(lineSplit.at(1));
            } else if (lineSplit.at(0) == "procs_blocked") {
                ret.processesBlocked = std::stol(lineSplit.at(1));
            } else if (lineSplit.at(0) == "softirq") {
                ret.softIRQ = std::stol(lineSplit.at(1));
            }
            //Ignore unrecognized key for forward compatibility
        }

        ret.total = getBytesFromKilobyte(memInfo["MemTotal"]);
        ret.free = getBytesFromKilobyte(memInfo["MemFree"]);
        ret.available = getBytesFromKilobyte(memInfo["MemAvailable"]);
        ret.buffers = getBytesFromKilobyte(memInfo["Buffers"]);
        ret.cached = getBytesFromKilobyte(memInfo["Cached"]);
        ret.active = getBytesFromKilobyte(memInfo["Active"]);
        ret.inactive = getBytesFromKilobyte(memInfo["Inactive"]);
        ret.anonActive = getBytesFromKilobyte(memInfo["Active(anon)"]);
        ret.anonInactive = getBytesFromKilobyte(memInfo["Inactive(anon)"]);
        ret.fileActive = getBytesFromKilobyte(memInfo["Active(file)"]);
        ret.fileInactive = getBytesFromKilobyte(memInfo["Inactive(file)"]);
        ret.unevictable = getBytesFromKilobyte(memInfo["Unevictable"]);
        ret.locked = getBytesFromKilobyte(memInfo["Mlocked"]);
        ret.highTotal = getBytesFromKilobyte(memInfo["HighTotal"]);
        ret.highFree = getBytesFromKilobyte(memInfo["HighFree"]);
        ret.lowTotal = getBytesFromKilobyte(memInfo["LowTotal"]);
        ret.lowFree = getBytesFromKilobyte(memInfo["LowFree"]);
        ret.mmapCopy = getBytesFromKilobyte(memInfo["MmapCopy"]);
        ret.swapTotal = getBytesFromKilobyte(memInfo["SwapTotal"]);
        ret.swapFree = getBytesFromKilobyte(memInfo["SwapFree"]);
        ret.dirty = getBytesFromKilobyte(memInfo["Dirty"]);
        ret.writeBack = getBytesFromKilobyte(memInfo["Writeback"]);
        ret.anonPages = getBytesFromKilobyte(memInfo["AnonPages"]);
        ret.mapped = getBytesFromKilobyte(memInfo["Mapped"]);
        ret.shmem = getBytesFromKilobyte(memInfo["Shmem"]);
        ret.kreclaimable = getBytesFromKilobyte(memInfo["KReclaimable"]);
        ret.slab = getBytesFromKilobyte(memInfo["Slab"]);
        ret.sreclaimable = getBytesFromKilobyte(memInfo["SReclaimable"]);
        ret.sunreclaim = getBytesFromKilobyte(memInfo["SUnreclaim"]);
        ret.kernelStack = getBytesFromKilobyte(memInfo["KernelStack"]);
        ret.pageTables = getBytesFromKilobyte(memInfo["PageTables"]);
        ret.quickLists = getBytesFromKilobyte(memInfo["Quicklists"]);
        ret.nfsUnstable = getBytesFromKilobyte(memInfo["NFS_Unstable"]);
        ret.bounce = getBytesFromKilobyte(memInfo["Bounce"]);
        ret.writebackTmp = getBytesFromKilobyte(memInfo["WritebackTmp"]);
        ret.commitLimit = getBytesFromKilobyte(memInfo["CommitLimit"]);
        ret.commitedAs = getBytesFromKilobyte(memInfo["Committed_AS"]);
        ret.vmAllocTotal = getBytesFromKilobyte(memInfo["VmallocTotal"]);
        ret.vmAllocUsed = getBytesFromKilobyte(memInfo["VmallocUsed"]);
        ret.vmAllocChunk = getBytesFromKilobyte(memInfo["VmallocChunk"]);
        ret.hardwareCorrupted = getBytesFromKilobyte(memInfo["HardwareCorrupted"]);
        ret.lazyFree = getBytesFromKilobyte(memInfo["LazyFree"]);
        ret.anonHugePages = getBytesFromKilobyte(memInfo["AnonHugePages"]);
        ret.shmemHugePages = getBytesFromKilobyte(memInfo["ShmemHugePages"]);
        ret.shmemPmdMapped = getBytesFromKilobyte(memInfo["ShmemPmdMapped"]);
        ret.cmaTotal = getBytesFromKilobyte(memInfo["CmaTotal"]);
        ret.cmaFree = getBytesFromKilobyte(memInfo["CmaFree"]);
        ret.hugePagesTotal = getBytesFromKilobyte(memInfo["HugePages_Total"]);
        ret.hugePagesFree = getBytesFromKilobyte(memInfo["HugePages_Free"]);
        ret.hugePagesRsvd = getBytesFromKilobyte(memInfo["HugePages_Rsvd"]);
        ret.hugePagesSurp = getBytesFromKilobyte(memInfo["HugePages_Surp"]);
        ret.hugePageSize = getBytesFromKilobyte(memInfo["Hugepagesize"]);
        ret.directMap4K = getBytesFromKilobyte(memInfo["DirectMap4k"]);
        ret.directMap4M = getBytesFromKilobyte(memInfo["DirectMap4M"]);
        ret.directMap2M = getBytesFromKilobyte(memInfo["DirectMap2M"]);
        ret.directMap1G = getBytesFromKilobyte(memInfo["DirectMap1G"]);

        return ret;
    }

    std::map<Pid_t, Process> readProcesses() {
        std::map<Pid_t, Process> ret;
        for (auto &fl : std::filesystem::directory_iterator(ProcPath::getProcDirectory())) {
            try {
                auto filename = fl.path().filename();
                if (ProcReader::isPID(filename)) {
                    auto pid = stringToPid(filename);
                    ret[pid] = ProcReader::readProcess(pid);
                }
            }
            catch (const std::exception &e) {} // Assume process does not exist anymore
        }
        return ret;
    }
}