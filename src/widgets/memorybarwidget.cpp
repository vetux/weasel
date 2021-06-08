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

#include "widgets/memorybarwidget.hpp"

#include <sstream>
#include <iomanip>

#include <QHBoxLayout>

MemoryBarWidget::MemoryBarWidget() {
    auto l = new QHBoxLayout();
    l->setMargin(0);
    memoryFreeBar = new QProgressBar();
    memoryAvailableBar = new QProgressBar();
    memoryText = new QLabel();
    memoryText->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    auto al = new QHBoxLayout();
    al->setMargin(0);
    al->setContentsMargins(0, 0, 5, 0);
    al->addWidget(memoryText);
    memoryAvailableBar->setLayout(al);

    auto ml = new QHBoxLayout();
    ml->setMargin(0);
    ml->addWidget(memoryAvailableBar);
    memoryFreeBar->setLayout(ml);
    memoryFreeBar->setStyleSheet(
            "QProgressBar {background:transparent; color:transparent;} QProgressBar::chunk { background-color: orange; width: 2px; margin-left: 1px; margin-right: 1px;}");
    memoryAvailableBar->setStyleSheet(
            "QProgressBar {background:transparent; color:transparent;} QProgressBar::chunk { background-color: green; width: 2px;  margin-left: 1px; margin-right: 1px;}");
    memoryAvailableBar->setRange(0, 100);
    memoryFreeBar->setRange(0, 100);
    l->addWidget(memoryFreeBar);
    setLayout(l);
}

void MemoryBarWidget::setMemory(const Memory &mem) {
    float availableScale = static_cast<float>(mem.total - mem.available) / mem.total;
    float freeScale = static_cast<float>(mem.total - mem.free) / mem.total;

    memoryAvailableBar->setValue(static_cast<int>(100 * availableScale));
    memoryFreeBar->setValue(static_cast<int>(100 * freeScale));

    std::string str;

    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << (float) (mem.total - mem.available) / (float) 1000000000;
    str += stream.str() + "G/";

    stream = {};
    stream << std::fixed << std::setprecision(2) << (float) mem.total / (float) 1000000000;
    str += stream.str() + "G";

    memoryText->setText(str.c_str());
}