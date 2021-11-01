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

#ifndef WEASEL_CPUBARWIDGET_HPP
#define WEASEL_CPUBARWIDGET_HPP

#include <qt5/QtWidgets/QProgressBar>
#include <qt5/QtWidgets/QLabel>

#include "weasel/cpustate.hpp"

class CpuBarWidget : public QWidget {
Q_OBJECT
public:
    explicit CpuBarWidget(QWidget *parent = nullptr);

public slots:
    void setCpus(const std::vector<CpuState::Core> &cores);

private:
    std::vector<QProgressBar *> cpuBars;
    std::vector<QLabel *> cpuLabels;
    std::vector<QLabel *> percentageLabels;
};

#endif //WEASEL_CPUBARWIDGET_HPP
