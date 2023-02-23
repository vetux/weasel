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

#include "filebrowser.hpp"

#include <cstdlib>

static std::string getDirectory(const std::filesystem::path &path) {
    std::string ret;
    if (std::filesystem::is_directory(path)) {
        ret = path.string();
    } else {
        if (!path.has_relative_path()) {
            throw std::runtime_error("Invalid path " + path.string() + " (No relative path)");
        }
        ret = path.parent_path().string();
        if (ret.empty()) {
            throw std::runtime_error("Invalid path " + path.string() + " (No parent path)");
        }
    }
    if (!std::filesystem::exists(ret)) {
        throw std::runtime_error("Invalid path '" + path.string() + "' (File not found)");
    }
    return ret;
}

static std::string CMD_OPEN_DETACHED(const std::string &path) {
    return "nohup xdg-open " + path + " > /dev/null 2> /dev/null &";
}

static std::string CMD_OPEN_BLOCKING(const std::string &path) {
    return "xdg-open " + path;
}

void FileBrowser::openDetached(const std::filesystem::path &path) {
    auto dir = getDirectory(path);
    auto cmd = CMD_OPEN_DETACHED(dir);
    auto code = std::system(cmd.c_str());
    if (code != 0) {
        throw std::runtime_error("Failed to run command. Exit Code: " + std::to_string(code));
    }
}

void FileBrowser::openBlocking(const std::filesystem::path &path) {
    auto dir = getDirectory(path);
    auto cmd = CMD_OPEN_BLOCKING(dir);
    auto code = std::system(cmd.c_str());
    if (code != 0) {
        throw std::runtime_error("Failed to run command. Exit Code: " + std::to_string(code));
    }
}
