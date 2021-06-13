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

#include "core/signal.hpp"

std::string signalToString(Signal sig) {
    switch (sig) {
        case SIGNAL_SIGHUP:
            return "SIGHUP";
        case SIGNAL_SIGINT:
            return "SIGINT";
        case SIGNAL_SIGQUIT:
            return "SIGQUIT";
        case SIGNAL_SIGILL:
            return "SIGILL";
        case SIGNAL_SIGABRT:
            return "SIGABRT";
        case SIGNAL_SIGFPE:
            return "SIGFPE";
        case SIGNAL_SIGKILL:
            return "SIGKILL";
        case SIGNAL_SIGSEGV:
            return "SIGSEGV";
        case SIGNAL_SIGPIPE:
            return "SIGPIPE";
        case SIGNAL_SIGALRM:
            return "SIGALRM";
        case SIGNAL_SIGTERM:
            return "SIGTERM";
        case SIGNAL_SIGUSR1:
            return "SIGUSR1";
        case SIGNAL_SIGUSR2:
            return "SIGUSR2";
        case SIGNAL_SIGCHLD:
            return "SIGCHLD";
        case SIGNAL_SIGCONT:
            return "SIGCONT";
        case SIGNAL_SIGSTOP:
            return "SIGSTOP";
        case SIGNAL_SIGTSTP:
            return "SIGTSTP";
        case SIGNAL_SIGTTIN:
            return "SIGTTIN";
        case SIGNAL_SIGTTOU:
            return "SIGTTOU";
        default:
            throw std::runtime_error("Unrecognized signal " + std::to_string(sig));
    }
}

Signal stringToSignal(const std::string &str) {
    if (str == "SIGHUP")
        return SIGNAL_SIGHUP;
    else if (str == "SIGINT")
        return SIGNAL_SIGINT;
    else if (str == "SIGQUIT")
        return SIGNAL_SIGQUIT;
    else if (str == "SIGILL")
        return SIGNAL_SIGILL;
    else if (str == "SIGABRT")
        return SIGNAL_SIGABRT;
    else if (str == "SIGFPE")
        return SIGNAL_SIGFPE;
    else if (str == "SIGKILL")
        return SIGNAL_SIGKILL;
    else if (str == "SIGSEGV")
        return SIGNAL_SIGSEGV;
    else if (str == "SIGPIPE")
        return SIGNAL_SIGPIPE;
    else if (str == "SIGALRM")
        return SIGNAL_SIGALRM;
    else if (str == "SIGTERM")
        return SIGNAL_SIGTERM;
    else if (str == "SIGUSR1")
        return SIGNAL_SIGUSR1;
    else if (str == "SIGUSR2")
        return SIGNAL_SIGUSR2;
    else if (str == "SIGCHLD")
        return SIGNAL_SIGCHLD;
    else if (str == "SIGCONT")
        return SIGNAL_SIGCONT;
    else if (str == "SIGSTOP")
        return SIGNAL_SIGSTOP;
    else if (str == "SIGTSTP")
        return SIGNAL_SIGTSTP;
    else if (str == "SIGTTIN")
        return SIGNAL_SIGTTIN;
    else if (str == "SIGTTOU")
        return SIGNAL_SIGTTOU;
    else
        throw std::runtime_error("Unrecognized signal " + str);
}
