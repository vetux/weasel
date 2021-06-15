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

#include "system/user.hpp"

#include <string>
#include <stdexcept>

#include <cstring>

#include <pwd.h>
#include <unistd.h>

void User::setUser(Uid_t uid) {
    if (setuid(uid) == -1) {
        auto err = errno;
        throw std::runtime_error("Failed to set user: " + std::string(strerror(err)));
    }
}

Uid_t User::getUser() {
    return getuid();
}

Uid_t User::getEffectiveUser() {
    return geteuid();
}

std::string User::getUserName(Uid_t uid) {
    return getpwuid(uid)->pw_name;
}

Uid_t User::getUserId(const std::string &userName) {
    return getpwnam(userName.c_str())->pw_uid;
}
