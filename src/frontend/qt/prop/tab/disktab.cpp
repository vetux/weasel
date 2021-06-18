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
#include <QLineEdit>

DiskTab::DiskTab(QWidget *parent)
        : QWidget(parent) {
    openFilesTitleLabel = new QLabel(this);
    openFilesListWidget = new QListWidget(this);
    openFilesPushButton = new QPushButton(this);

    statReadBytesTitleLabel = new QLabel(this);
    statReadBytesLabel = new QLineEdit(this);

    statWriteBytesTitleLabel = new QLabel(this);
    statWriteBytesLabel = new QLineEdit(this);

    openFilesTitleLabel->setText("Open Files");
    openFilesPushButton->setText("Open");

    statReadBytesTitleLabel->setText("I/O Read");
    statWriteBytesTitleLabel->setText("I/O Write");

    statReadBytesLabel->setReadOnly(true);
    statWriteBytesLabel->setReadOnly(true);

    auto metrics = QFontMetrics(statWriteBytesTitleLabel->font());
    auto r = metrics.boundingRect(statWriteBytesTitleLabel->text());

    statReadBytesTitleLabel->setMinimumWidth(r.width());
    statWriteBytesTitleLabel->setMinimumWidth(r.width());

    setLayout(new QVBoxLayout());
    layout()->addWidget(openFilesTitleLabel);
    layout()->addWidget(openFilesListWidget);
    layout()->addWidget(openFilesPushButton);

    auto *layoutWidget = new QWidget(this);
    layoutWidget->setLayout(new QHBoxLayout());
    layoutWidget->layout()->setMargin(0);

    layoutWidget->layout()->addWidget(statReadBytesTitleLabel);
    (dynamic_cast<QHBoxLayout *>(layoutWidget->layout()))->addWidget(statReadBytesLabel, 1);

    layout()->addWidget(layoutWidget);

    layoutWidget = new QWidget(this);
    layoutWidget->setLayout(new QHBoxLayout());
    layoutWidget->layout()->setMargin(0);

    layoutWidget->layout()->addWidget(statWriteBytesTitleLabel);
    (dynamic_cast<QHBoxLayout *>(layoutWidget->layout()))->addWidget(statWriteBytesLabel, 1);

    layout()->addWidget(layoutWidget);

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

    statReadBytesLabel->setText(std::to_string(proc.rchar).c_str());
    statWriteBytesLabel->setText(std::to_string(proc.wchar).c_str());
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

    statReadBytesLabel->setText(std::to_string(proc.rchar).c_str());
    statWriteBytesLabel->setText(std::to_string(proc.wchar).c_str());
}

void DiskTab::clearData() {
    openFilesListWidget->clear();
    statReadBytesLabel->setText("");
    statWriteBytesLabel->setText("");
}

void DiskTab::onOpenFilePressed() {
    auto item = openFilesListWidget->currentItem();
    if (item != nullptr) {
        auto f = item->text().toStdString();
    }
}
