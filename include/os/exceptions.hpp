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

#ifndef WEASEL_EXCEPTIONS_HPP
#define WEASEL_EXCEPTIONS_HPP

#include <stdexcept>

class PermissionError : public std::runtime_error {
public:
    explicit PermissionError(const std::string &message = "") : std::runtime_error(message) {};
};

class ProcessNotFound : public std::runtime_error {
public:
    explicit ProcessNotFound(const std::string &message = "") : std::runtime_error(message) {};
};

class ThreadNotFound : public std::runtime_error {
public:
    explicit ThreadNotFound(const std::string &message = "") : std::runtime_error(message) {};
};

#endif //WEASEL_EXCEPTIONS_HPP
