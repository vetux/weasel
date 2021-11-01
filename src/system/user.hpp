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

#ifndef WEASEL_USER_HPP
#define WEASEL_USER_HPP

#include <string>

#include "system/types.hpp"

namespace User {
    /**
     * Set the effective uid of the calling process.
     *
     * @return
     */
    void setUser(Uid_t uid);

    /**
     * Returns the real uid of the current user.
     *
     * @return The uid of the user executing the calling process.
     */
    Uid_t getUser();

    /**
     * Returns the effective uid of the current user.
     *
     * @return The effective uid of the user executing the calling process.
     */
    Uid_t getEffectiveUser();

    /**
     * Find the username for a given uid.
     *
     * @param pid The uid of the user to find the name of
     * @return The name of the user with the pid
     */
    std::string getUserName(Uid_t pid);

    /**
     * Find the uid for a given name.
     *
     * @param userName The name of the user to find the uid of
     * @return The uid of the user with the name
     */
    Uid_t getUserId(const std::string &userName);
}

#endif //WEASEL_USER_HPP
