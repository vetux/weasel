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

#include "weasel/fileio.hpp"

#include <fstream>
#include <sstream>

#include <cstring>

namespace FileIO {
    std::string readText(const std::string &filePath, const std::string &newLine) {
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
}