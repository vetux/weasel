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

#ifndef WEASEL_BYTEFORMATTING_HPP
#define WEASEL_BYTEFORMATTING_HPP

#include <string>

#define BYTES_KILOBYTE 1000
#define BYTES_MEGABYTE 1000000
#define BYTES_GIGABYTE 1000000000

inline std::string getFormattedRateString(unsigned long bytesPerSecond) {
    if (bytesPerSecond < BYTES_KILOBYTE) {
        return std::to_string(bytesPerSecond) + " B/s";
    } else if (bytesPerSecond < BYTES_MEGABYTE) {
        return std::to_string(bytesPerSecond / BYTES_KILOBYTE) + " KB/s";
    } else if (bytesPerSecond < BYTES_GIGABYTE) {
        return std::to_string(bytesPerSecond / BYTES_MEGABYTE) + " MB/s";
    } else {
        return std::to_string(bytesPerSecond / BYTES_GIGABYTE) + " GB/s";
    }
}

static std::string getFormattedSizeString(unsigned long bytes) {
    if (bytes >= BYTES_GIGABYTE) {
        return std::to_string(bytes / BYTES_GIGABYTE) + " GB";
    } else if (bytes >= BYTES_MEGABYTE) {
        return std::to_string(bytes / BYTES_MEGABYTE) + " MB";
    }else if(bytes >= BYTES_KILOBYTE){
        return std::to_string(bytes / BYTES_KILOBYTE) + " KB";
    } else {
        return std::to_string(bytes) + " B";
    }
}

#endif //WEASEL_BYTEFORMATTING_HPP
