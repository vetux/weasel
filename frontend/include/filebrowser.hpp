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

#ifndef WEASEL_FILEBROWSER_HPP
#define WEASEL_FILEBROWSER_HPP

#include <filesystem>

namespace FileBrowser {
    /**
     * Runs "nohup xdg-open PATH &"
     *
     * If the file path points to a file the corresponding parent directory path is passed to xdg-open
     * to ensure that xdg-open opens the file browser.
     *
     * @param path
     */
    void openDetached(const std::filesystem::path &path);

    void openBlocking(const std::filesystem::path &path);
}
#endif //WEASEL_FILEBROWSER_HPP
