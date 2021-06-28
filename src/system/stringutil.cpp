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

#include "system/stringutil.hpp"

namespace StringUtil {
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
}