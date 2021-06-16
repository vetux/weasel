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

#include "frontend/qt/widget/toolbarwidget.hpp"

#include <sstream>
#include <iomanip>

ToolbarWidget::ToolbarWidget(QWidget *parent) : QWidget(parent) {
    setLayout(new QHBoxLayout());

    refreshButton = new QPushButton();
    memBar = new MemoryBarWidget();
    cpuBar = new CpuBarWidget();

    refreshButton->setText("Refresh");

    layout()->addWidget(refreshButton);
    layout()->addWidget(cpuBar);
    layout()->addWidget(memBar);
    layout()->setMargin(0);

    connect(refreshButton, SIGNAL(pressed()), this, SIGNAL(refreshPressed()));
}

ToolbarWidget::~ToolbarWidget() {

}

void ToolbarWidget::setSystemStatus(const SystemStatus &system, const SystemStatus &prevSystem) {
    float available = static_cast<float>(system.available) / system.total;
    float free = static_cast<float>(system.free) / system.total;

    memBar->setAvailable(available);
    memBar->setFree(free);

    std::string text;

    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << (float) (system.total - system.available) / (float) 1000000000;
    text += stream.str() + "G/";

    stream = {};
    stream << std::fixed << std::setprecision(2) << (float) system.total / (float) 1000000000;
    text += stream.str() + "G";

    memBar->setText(text.c_str());

    cpuBar->setTotalCpu(system.cpu, prevSystem.cpu);
    cpuBar->setCpus(system.cores, prevSystem.cores);
}
