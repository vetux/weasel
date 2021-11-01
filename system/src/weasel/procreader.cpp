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

#include "weasel/procreader.hpp"

#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

#include <cstdlib>
#include <cstring>
#include <cassert>

#include <sys/stat.h>

#include "weasel/procpath.hpp"
#include "weasel/syscalls.hpp"
#include "weasel/procnetreader.hpp"
#include "weasel/fileio.hpp"
#include "weasel/stringutil.hpp"
#include "weasel/user.hpp"
#include "weasel/strformat.hpp"

#define READLINK_BUFFER_SIZE 512

inline bool isAsciiNumber(char c) {
    return c >= '0' && c <= '9';
}

Mem_t getBytesFromKilobyte(const std::string &mem) {
    auto str = mem.substr(0, mem.find(' '));
    return stringToMem(str) * 1000;
}

Thread::SchedulingPolicy convertPolicy(uint32_t policy) {
    switch (policy) {
        case SCHED_OTHER:
            return Thread::OTHER;
        case SCHED_BATCH:
            return Thread::BATCH;
        case SCHED_IDLE:
            return Thread::IDLE;
        case SCHED_FIFO:
            return Thread::FIFO;
        case SCHED_RR:
            return Thread::RR;
        case SCHED_DEADLINE:
            return Thread::DEADLINE;
        default:
            throw std::runtime_error("Invalid policy value");
    }
}

std::string getRealPath(const std::string &symLinkPath) {
    auto *p = realpath(symLinkPath.c_str(), NULL);
    if (p == NULL)
        throw std::runtime_error("Failed to get real path for " + symLinkPath);
    return p;
}

std::string readLink(const std::string &link) {
    char *l = new char[READLINK_BUFFER_SIZE];
    memset(l, 0, READLINK_BUFFER_SIZE);
    auto r = readlink(link.c_str(), l, READLINK_BUFFER_SIZE);
    if (r == -1) {
        delete[]l;
        throw std::runtime_error("Failed to read link at " + link);
    }
    std::string ret(l, r);
    delete[]l;
    return ret;
}

Socket parseSocketString(const std::string &socketString, const std::map<Inode_t, Socket> &netStat) {
    auto prefixLength = std::string("socket:[").size();
    auto inode = stringToInode(socketString.substr(prefixLength, socketString.size() - 1 - prefixLength));
    for (auto &pair: netStat) {
        if (pair.first == inode) {
            return pair.second;
        }
    }
    throw std::runtime_error("Failed to find socket with inode " + std::to_string(inode));
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

        StringUtil::replace(value, "\t", " ");
        StringUtil::replace(value, "  ", " ");

        value = StringUtil::removeSurroundingWhiteSpace(value);

        ret[name] = value;
    }

    return ret;
}

void parseFileDescriptor(Process &proc, const std::filesystem::path &path, const std::map<Inode_t, Socket> &netStat) {
    // Check for stdin / stdout / stderr file descriptor names because they are hardcoded and can be assumed to be present for all processes.
    if (path.filename() == "0" || path.filename() == "1" || path.filename() == "2") {
        return;
    }

    std::string symLinkPath = readLink(path);

    // Check if symlink points to a file or something else.
    // (If a file is accessible with the name socket:[XXX] this obviously will give false positives
    // but there does not seem to be a way to stat a file descriptor of another process.
    if (access(symLinkPath.c_str(), F_OK) == 0) {
        //Assume the file descriptor refers to a file
        proc.files.emplace_back(getRealPath(symLinkPath));
    } else {
        if (symLinkPath.find("socket:[") == 0) {
            //Assume the file descriptor refers to a socket
            try {
                proc.sockets.emplace_back(parseSocketString(symLinkPath, netStat));
            }
            catch (std::exception &e) {} //Assume socket dead
        }
    }
}

void parseProcessCmdline(Process &proc) {
    try {
        proc.commandLine = FileIO::readText(ProcPath::getProcessCommandLineFile(proc.pid), " ");
    } catch (const std::exception &e) {} //Assume permissions error
}

void parseProcessEnviron(Process &proc) {
    try {
        auto s = FileIO::readText(ProcPath::getProcessEnvironFile(proc.pid));
        StringUtil::replace(s, "\n", "");
        proc.environ = StringUtil::splitString(s, std::string("\0", 1));
    } catch (const std::exception &e) {} //Assume permissions error
}

void parseProcessRoot(Process &proc) {
    try {
        proc.rootDirectory = getRealPath(ProcPath::getProcessRootDirectory(proc.pid));
    } catch (const std::exception &e) {} //Assume permissions error
}

void parseProcessFd(Process &proc, const std::map<Inode_t, Socket> &netStat) {
    try {
        const auto p = ProcPath::getProcessFdDirectory(proc.pid);
        for (auto &f: std::filesystem::directory_iterator(p)) {
            parseFileDescriptor(proc, f, netStat);
        }
    } catch (const std::exception &e) {} //Assume permissions error
}

void parseProcessExe(Process &proc) {
    try {
        proc.executablePath = getRealPath(ProcPath::getProcessExeDirectory(proc.pid));
    } catch (const std::exception &e) {} //Assume permissions error
}

void parseProcessStatm(Process &process) {
    try {
        auto split = StringUtil::splitString(FileIO::readText(ProcPath::getProcessStatmFile(process.pid)), " ");
        process.size = std::stoul(split.at(0));
        process.resident = std::stoul(split.at(1));
        process.shared = std::stoul(split.at(2));
        process.text = std::stoul(split.at(3));
        process.lib = std::stoul(split.at(4));
        process.data = std::stoul(split.at(5));
        process.dt = std::stoul(split.at(6));
    } catch (const std::exception &e) {} //Assume permissions error
}

void parseProcessCwd(Process &process) {
    try {
        process.cwd = getRealPath(ProcPath::getProcessCwdFile(process.pid));
    } catch (const std::exception &e) {} //Assume permissions error
}

void parseProcIo(Process &process) {
    try {
        const auto d = parseProcStr(FileIO::readText(ProcPath::getProcessIoFile(process.pid)));
        process.rchar = std::stoul(d.at("rchar"));
        process.wchar = std::stoul(d.at("wchar"));
        process.syscr = std::stoul(d.at("syscr"));
        process.syscw = std::stoul(d.at("syscw"));
        process.read_bytes = std::stoul(d.at("read_bytes"));
        process.write_bytes = std::stoul(d.at("write_bytes"));
        process.cancelled_write_bytes = std::stoul(d.at("cancelled_write_bytes"));
    } catch (const std::exception &e) {} //Assume permissions error
}

void parseProcessStat(Process &process) {
    try {
        auto text = FileIO::readText(ProcPath::getProcessStatFile(process.pid));
        auto firstBracket = text.find_first_of('(');
        auto lastBracket = text.find_last_of(')');

        process.comm = text.substr(firstBracket + 1, lastBracket - firstBracket - 1);

        text.erase(0, lastBracket + 1);

        auto split = StringUtil::splitString(text, " ");

        process.state = split.at(0)[0];
        process.ppid = std::stoi(split.at(1));
        process.pgrp = std::stoi(split.at(2));
        process.session = std::stoi(split.at(3));
        process.tty_nr = std::stoi(split.at(4));
        process.tpgid = std::stoi(split.at(5));
        process.flags = std::stoi(split.at(6));
        process.minflt = std::stoul(split.at(7));
        process.cminflt = std::stoul(split.at(8));
        process.majflt = std::stoul(split.at(9));
        process.cmajflt = std::stoul(split.at(10));
        process.utime = std::stoul(split.at(11));
        process.stime = std::stoul(split.at(12));
        process.cutime = std::stol(split.at(13));
        process.cstime = std::stol(split.at(14));
        process.priority = std::stol(split.at(15));
        process.nice = std::stol(split.at(16));
        process.num_threads = std::stol(split.at(17));
        process.itrealvalue = std::stol(split.at(18));
        process.starttime = std::stoull(split.at(19));
        process.vsize = std::stoul(split.at(20));
        process.rss = std::stol(split.at(21));
        process.rsslim = std::stoul(split.at(22));
        process.startcode = std::stoul(split.at(23));
        process.endcode = std::stoul(split.at(24));
        process.startstack = std::stoul(split.at(25));
        process.kstkesp = std::stoul(split.at(26));
        process.kstkeip = std::stoul(split.at(27));
        process.signal = std::stoul(split.at(28));
        process.blocked = std::stoul(split.at(29));
        process.sigignore = std::stoul(split.at(30));
        process.sigcatch = std::stoul(split.at(31));
        process.wchan = std::stoul(split.at(32));
        process.nswap = std::stoul(split.at(33));
        process.cnswap = std::stoul(split.at(34));

        if (split.size() < 36)
            return;

        process.exit_signal = std::stoi(split.at(35));

        if (split.size() < 37)
            return;

        process.processor = std::stoi(split.at(36));

        if (split.size() < 38)
            return;

        process.rt_priority = std::stoul(split.at(37));

        if (split.size() < 39)
            return;

        process.policy = convertPolicy(std::stoul(split.at(38)));

        if (split.size() < 40)
            return;

        process.delayacct_blkio_ticks = std::stoull(split.at(39));

        if (split.size() < 41)
            return;

        process.guest_time = std::stoul(split.at(40));

        if (split.size() < 42)
            return;

        process.cguest_time = std::stol(split.at(41));

        if (split.size() < 43)
            return;

        process.start_data = std::stoul(split.at(42));

        if (split.size() < 44)
            return;

        process.end_data = std::stoul(split.at(43));

        if (split.size() < 45)
            return;

        process.start_brk = std::stoul(split.at(44));

        if (split.size() < 46)
            return;

        process.arg_start = std::stoul(split.at(45));

        if (split.size() < 47)
            return;

        process.arg_end = std::stoul(split.at(46));

        if (split.size() < 48)
            return;

        process.env_start = std::stoul(split.at(47));

        if (split.size() < 49)
            return;

        process.env_end = std::stoul(split.at(48));

        if (split.size() < 50)
            return;

        process.exit_code = std::stoi(split.at(49));

    } catch (const std::exception &e) {} //Assume permissions error
}

void parseThreadStat(Thread &thread) {
    try {
        auto text = FileIO::readText(ProcPath::getThreadStatFile(thread.pid, thread.tid));
        auto firstBracket = text.find_first_of('(');
        auto lastBracket = text.find_last_of(')');

        thread.comm = text.substr(firstBracket + 1, lastBracket - firstBracket - 1);

        text.erase(0, lastBracket + 1);

        auto split = StringUtil::splitString(text, " ");

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

        thread.policy = convertPolicy(std::stoul(split.at(38)));

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

void parseThreadIo(Thread &thread) {
    try {
        const auto d = parseProcStr(FileIO::readText(ProcPath::getThreadIoFile(thread.pid, thread.tid)));
        thread.rchar = std::stoul(d.at("rchar"));
        thread.wchar = std::stoul(d.at("wchar"));
        thread.syscr = std::stoul(d.at("syscr"));
        thread.syscw = std::stoul(d.at("syscw"));
        thread.read_bytes = std::stoul(d.at("read_bytes"));
        thread.write_bytes = std::stoul(d.at("write_bytes"));
        thread.cancelled_write_bytes = std::stoul(d.at("cancelled_write_bytes"));
    } catch (const std::exception &e) {} //Assume permissions error
}

Uid_t getFileOwnerUid(const std::filesystem::path &p) {
    try {
        struct stat s{};
        stat(p.c_str(), &s);
        return s.st_uid;
    } catch (const std::exception &e) {} //Assume permissions error
}

CpuState::Core readCore(const std::vector<std::string> &line) {
    CpuState::Core ret{};
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

bool isPID(const std::string &name) {
    for (auto &c: name) {
        if (!isAsciiNumber(c))
            return false;
    }
    return true;
}

namespace ProcReader {
    CpuState readCpuState() {
        auto statContents = FileIO::readText(ProcPath::getProcStatFile());
        StringUtil::replace(statContents, "  ", " "); //replace double whitespace

        auto stat = StringUtil::readLines(statContents);

        CpuState ret;

        for (auto &line: stat) {
            auto lineSplit = StringUtil::splitString(line, " ");
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

        return ret;
    }

    MemoryState readMemoryState() {
        auto memInfo = parseProcStr(FileIO::readText(ProcPath::getMemoryInfoFile()));

        MemoryState ret{};

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

    std::map<Inode_t, Socket> readNetworkState() {
        return ProcNetReader::getNetworkState();
    }

    std::map<Pid_t, Process> readProcesses(const std::map<Inode_t, Socket> &netStat) {
        std::map<Pid_t, Process> ret;
        for (auto &fl: std::filesystem::directory_iterator(ProcPath::getProcDirectory())) {
            try {
                auto filename = fl.path().filename();
                if (isPID(filename)) {
                    auto pid = stringToPid(filename);
                    ret[pid] = ProcReader::readProcess(pid, netStat);
                }
            }
            catch (const std::exception &e) {} // Assume process does not exist anymore
        }
        return ret;
    }

    Process readProcess(Pid_t pid, const std::map<Inode_t, Socket> &netStat) {
        Process p;
        p.pid = pid;
        p.uid = getFileOwnerUid(ProcPath::getProcessDirectory(pid));
        p.userName = User::getUserName(p.uid);

        // Parse threads before parsing the process stat so that the process stat data
        // may contain additional cycles, etc which happened between reading thread data and process data
        // but never less.
        std::string dir = ProcPath::getProcessTasksDirectory(pid);
        for (auto &d: std::filesystem::directory_iterator(dir)) {
            auto t = readThread(pid, stringToPid(d.path().filename()));
            p.threads[t.tid] = readThread(pid, stringToPid(d.path().filename()));
        }

        parseProcessCmdline(p);
        parseProcessEnviron(p);
        parseProcessRoot(p);
        parseProcessFd(p, netStat);
        parseProcessExe(p);
        parseProcessCwd(p);
        parseProcessStat(p);
        parseProcessStatm(p);
        parseProcIo(p);

        return p;
    }

    Thread readThread(Pid_t pid, Pid_t tid) {
        Thread t;

        t.pid = pid;
        t.tid = tid;

        parseThreadStat(t);
        parseThreadIo(t);

        return t;
    }
}