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

#include "frontend/qt/prop/tab/disktab.hpp"

#include <QVBoxLayout>

DiskTab::DiskTab(QWidget *parent)
        : QWidget(parent) {
    openFilesTitleLabel = new QLabel(this);
    openFilesListWidget = new QListWidget(this);
    openFilesPushButton = new QPushButton(this);

    openFilesTitleLabel->setText("Open Files");
    openFilesPushButton->setText("Open");

    setLayout(new QVBoxLayout());
    layout()->addWidget(openFilesTitleLabel);
    layout()->addWidget(openFilesListWidget);
    layout()->addWidget(openFilesPushButton);

    connect(openFilesPushButton, SIGNAL(pressed()), this, SLOT(onOpenFilePressed()));
}

void DiskTab::setData(const SystemStatus &status,
                      const SystemStatus &prevStatus,
                      const Process &proc,
                      const Process &prevProc) {
    openFilesListWidget->clear();
    for (auto &f : proc.openFiles) {
        openFilesListWidget->addItem(f.c_str());
    }
}

void DiskTab::updateData(const SystemStatus &status,
                         const SystemStatus &prevStatus,
                         const Process &proc,
                         const Process &prevProc) {
    auto index = openFilesListWidget->currentIndex();
    openFilesListWidget->clear();
    for (auto &f : proc.openFiles) {
        openFilesListWidget->addItem(f.c_str());
    }
    openFilesListWidget->setCurrentIndex(index);
}

void DiskTab::clearData() {
    openFilesListWidget->clear();
}

void DiskTab::onOpenFilePressed() {
    auto item = openFilesListWidget->currentItem();
    if (item != nullptr) {
        auto f = item->text().toStdString();
    }
}
