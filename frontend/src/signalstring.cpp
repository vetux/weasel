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

#include <stdexcept>

#include "signalstring.hpp"

std::string signalToString(Thread::Signal sig) {
    switch (sig) {
        case Thread::SIGNAL_SIGHUP:
            return "SIGHUP";
        case Thread::SIGNAL_SIGINT:
            return "SIGINT";
        case Thread::SIGNAL_SIGQUIT:
            return "SIGQUIT";
        case Thread::SIGNAL_SIGILL:
            return "SIGILL";
        case Thread::SIGNAL_SIGABRT:
            return "SIGABRT";
        case Thread:: SIGNAL_SIGFPE:
            return "SIGFPE";
        case Thread::SIGNAL_SIGKILL:
            return "SIGKILL";
        case Thread::SIGNAL_SIGSEGV:
            return "SIGSEGV";
        case Thread::SIGNAL_SIGPIPE:
            return "SIGPIPE";
        case Thread::SIGNAL_SIGALRM:
            return "SIGALRM";
        case Thread::SIGNAL_SIGTERM:
            return "SIGTERM";
        case Thread::SIGNAL_SIGUSR1:
            return "SIGUSR1";
        case Thread::SIGNAL_SIGUSR2:
            return "SIGUSR2";
        case Thread::SIGNAL_SIGCHLD:
            return "SIGCHLD";
        case Thread::SIGNAL_SIGCONT:
            return "SIGCONT";
        case Thread::SIGNAL_SIGSTOP:
            return "SIGSTOP";
        case Thread::SIGNAL_SIGTSTP:
            return "SIGTSTP";
        case Thread::SIGNAL_SIGTTIN:
            return "SIGTTIN";
        case Thread::SIGNAL_SIGTTOU:
            return "SIGTTOU";
        default:
            throw std::runtime_error("Unrecognized signal " + std::to_string(sig));
    }
}

Thread::Signal stringToSignal(const std::string &str) {
    if (str == "SIGHUP")
        return Thread::SIGNAL_SIGHUP;
    else if (str == "SIGINT")
        return Thread::SIGNAL_SIGINT;
    else if (str == "SIGQUIT")
        return Thread::SIGNAL_SIGQUIT;
    else if (str == "SIGILL")
        return Thread::SIGNAL_SIGILL;
    else if (str == "SIGABRT")
        return Thread::SIGNAL_SIGABRT;
    else if (str == "SIGFPE")
        return Thread::SIGNAL_SIGFPE;
    else if (str == "SIGKILL")
        return Thread::SIGNAL_SIGKILL;
    else if (str == "SIGSEGV")
        return Thread::SIGNAL_SIGSEGV;
    else if (str == "SIGPIPE")
        return Thread::SIGNAL_SIGPIPE;
    else if (str == "SIGALRM")
        return Thread::SIGNAL_SIGALRM;
    else if (str == "SIGTERM")
        return Thread::SIGNAL_SIGTERM;
    else if (str == "SIGUSR1")
        return Thread::SIGNAL_SIGUSR1;
    else if (str == "SIGUSR2")
        return Thread::SIGNAL_SIGUSR2;
    else if (str == "SIGCHLD")
        return Thread::SIGNAL_SIGCHLD;
    else if (str == "SIGCONT")
        return Thread::SIGNAL_SIGCONT;
    else if (str == "SIGSTOP")
        return Thread::SIGNAL_SIGSTOP;
    else if (str == "SIGTSTP")
        return Thread::SIGNAL_SIGTSTP;
    else if (str == "SIGTTIN")
        return Thread::SIGNAL_SIGTTIN;
    else if (str == "SIGTTOU")
        return Thread::SIGNAL_SIGTTOU;
    else
        throw std::runtime_error("Unrecognized signal " + str);
}
