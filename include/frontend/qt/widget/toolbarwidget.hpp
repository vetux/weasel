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

#include <QWidget>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "system/systemstatus.hpp"

#include "frontend/qt/widget/memorybarwidget.hpp"

class ToolbarWidget : public QWidget {
Q_OBJECT
signals:

    void refreshPressed();

public:
    explicit ToolbarWidget(QWidget *parent = nullptr);

    ~ToolbarWidget() override;

    void setSystemStatus(const SystemStatus &system);

private:
    QPushButton *refreshButton;
    MemoryBarWidget *memBar;
};

#endif //WEASEL_TOOLBARWIDGET_HPP
