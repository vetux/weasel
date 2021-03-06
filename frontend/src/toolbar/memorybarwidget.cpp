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

#include "toolbar/memorybarwidget.hpp"

#include <sstream>
#include <iomanip>

#include <QHBoxLayout>

#define STYLE(color) "QProgressBar {background:transparent; color:transparent;} QProgressBar::chunk { background-color: "#color"; width: 2px; margin-left: 1px; margin-right: 1px;}"

MemoryBarWidget::MemoryBarWidget() {
    memoryAvailableBar = new QProgressBar();
    memoryText = new QLabel();

    memoryText->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    memoryAvailableBar->setRange(0, 100);

    memoryAvailableBar->setLayout(new QHBoxLayout());
    memoryAvailableBar->layout()->setMargin(0);
    memoryAvailableBar->layout()->setContentsMargins(0, 0, 5, 0);
    memoryAvailableBar->layout()->addWidget(memoryText);

    setLayout(new QHBoxLayout());
    layout()->setMargin(0);
    layout()->addWidget(memoryAvailableBar);
}

void MemoryBarWidget::setAvailable(float availableMemory) {
    memoryAvailableBar->setValue(static_cast<int>(100 * (1.0f - availableMemory)));
}

void MemoryBarWidget::setText(const QString &text) {
    memoryText->setText(text);
}