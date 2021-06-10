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

#ifndef WEASEL_MEMORYBARWIDGET_HPP
#define WEASEL_MEMORYBARWIDGET_HPP

#include <QWidget>
#include <QProgressBar>
#include <QLabel>

class MemoryBarWidget : public QWidget {
public:
    MemoryBarWidget();

    void setFree(float freeMemory);

    void setAvailable(float availableMemory);

    void setText(const QString &text);

private:
    QProgressBar *memoryFreeBar;
    QProgressBar *memoryAvailableBar;
    QLabel *memoryText;
};

#endif //WEASEL_MEMORYBARWIDGET_HPP
