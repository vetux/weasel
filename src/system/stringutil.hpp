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

#ifndef WEASEL_STRINGUTIL_HPP
#define WEASEL_STRINGUTIL_HPP

#include <string>
#include <vector>

namespace StringUtil {
    std::string removeSurroundingWhiteSpace(const std::string &str);

    std::string removeExtraWhitespace(const std::string &str);

    void replace(std::string &str, const std::string &v, const std::string &r);

    std::vector<std::string> splitString(const std::string &str, const std::string &delimiter);

    std::vector<std::string> readLines(const std::string &str);
}

#endif //WEASEL_STRINGUTIL_HPP
