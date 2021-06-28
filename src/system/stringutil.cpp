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

#include <sstream>

namespace StringUtil {
    std::string removeSurroundingWhiteSpace(const std::string &input) {
        //Preceding
        auto str = input;
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
        return str;
    }

    std::string removeExtraWhitespace(const std::string &str) {
        std::string ret;
        for (int i = 0; i < str.size(); i++) {
            ret += str.at(i);
            if (str.at(i) == ' ') {
                while (i + 1 < str.size() && str.at(i + 1) == ' ') {
                    i++;
                }
            }
        }
        return ret;
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

    std::vector<std::string> readLines(const std::string &str) {
        std::vector<std::string> ret;
        std::stringstream stream(str);

        std::string tmp;
        while (std::getline(stream, tmp)) {
            ret.emplace_back(tmp);
        }

        return ret;
    }
}