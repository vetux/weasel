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

#ifndef WEASEL_TOOLBARWIDGET_HPP
#define WEASEL_TOOLBARWIDGET_HPP

#include <qt5/QtWidgets/QWidget>
#include <qt5/QtWidgets/QProgressBar>
#include <qt5/QtWidgets/QHBoxLayout>
#include <qt5/QtWidgets/QLabel>
#include <qt5/QtWidgets/QPushButton>

#include "weasel/snapshot.hpp"

#include "memorybarwidget.hpp"
#include "cpubarwidget.hpp"

class ToolbarWidget : public QWidget {
Q_OBJECT
public:
    explicit ToolbarWidget(QWidget *parent = nullptr);

    ~ToolbarWidget() override;

public slots:

    void onSnapshot(const Snapshot &snapshot);

private:
    MemoryBarWidget *memBar;
    CpuBarWidget *cpuBar;
};

#endif //WEASEL_TOOLBARWIDGET_HPP