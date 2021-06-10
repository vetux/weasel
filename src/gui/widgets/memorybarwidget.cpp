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

#include "gui/widgets/memorybarwidget.hpp"

#include <sstream>
#include <iomanip>

#include <QHBoxLayout>

#define STYLE(color) "QProgressBar {background:transparent; color:transparent;} QProgressBar::chunk { background-color: "#color"; width: 2px; margin-left: 1px; margin-right: 1px;}"

MemoryBarWidget::MemoryBarWidget() {
    memoryFreeBar = new QProgressBar();
    memoryAvailableBar = new QProgressBar();
    memoryText = new QLabel();

    memoryText->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    memoryFreeBar->setStyleSheet(STYLE("orange"));
    memoryAvailableBar->setStyleSheet(STYLE("green"));

    memoryAvailableBar->setRange(0, 100);
    memoryFreeBar->setRange(0, 100);

    memoryAvailableBar->setLayout(new QHBoxLayout());
    memoryAvailableBar->layout()->setMargin(0);
    memoryAvailableBar->layout()->setContentsMargins(0, 0, 5, 0);
    memoryAvailableBar->layout()->addWidget(memoryText);

    memoryFreeBar->setLayout(new QHBoxLayout());
    memoryFreeBar->layout()->setMargin(0);
    memoryFreeBar->layout()->addWidget(memoryAvailableBar);

    setLayout(new QHBoxLayout());
    layout()->setMargin(0);
    layout()->addWidget(memoryFreeBar);
}

void MemoryBarWidget::setAvailable(float availableMemory) {
    memoryAvailableBar->setValue(static_cast<int>(100 * (1.0f - availableMemory)));
}

void MemoryBarWidget::setFree(float freeMemory) {
    memoryFreeBar->setValue(static_cast<int>(100 * (1.0f - freeMemory)));
}

void MemoryBarWidget::setText(const QString &text) {
    memoryText->setText(text);
}