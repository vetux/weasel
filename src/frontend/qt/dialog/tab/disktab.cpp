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

#include "frontend/qt/dialog/tab/disktab.hpp"

#include <QVBoxLayout>
#include <QLineEdit>
#include <QSpacerItem>

unsigned long getRate(float delta,
                      unsigned long current,
                      unsigned long previous) {
    auto diff = current - previous;
    if (diff == 0) {
        return 0;
    } else {
        auto scale = delta == 0 ? 0 : 1 / delta;
        return static_cast<unsigned long>(diff * scale);
    }
}

DiskTab::DiskTab(QWidget *parent)
        : QWidget(parent) {
    openFilesTitleLabel = new QLabel(this);
    openFilesListWidget = new QListWidget(this);
    openFilesPushButton = new QPushButton(this);

    statReadBytesTitleLabel = new QLabel(this);
    statReadBytesLabel = new QLineEdit(this);

    statReadBytesRateTitleLabel = new QLabel(this);
    statReadBytesRateLabel = new QLineEdit(this);

    statWriteBytesTitleLabel = new QLabel(this);
    statWriteBytesLabel = new QLineEdit(this);

    statWriteBytesRateTitleLabel = new QLabel(this);
    statWriteBytesRateLabel = new QLineEdit(this);

    openFilesTitleLabel->setText("Open Files");
    openFilesPushButton->setText("Open");

    statReadBytesTitleLabel->setText("Read Total");
    statReadBytesRateTitleLabel->setText("Read Rate");

    statWriteBytesTitleLabel->setText("Write Total");
    statWriteBytesRateTitleLabel->setText("Write Rate");

    statReadBytesLabel->setReadOnly(true);
    statWriteBytesLabel->setReadOnly(true);

    auto metrics = QFontMetrics(statWriteBytesTitleLabel->font());
    auto r = metrics.boundingRect(statWriteBytesTitleLabel->text());

    statReadBytesTitleLabel->setMinimumWidth(r.width());
    statReadBytesRateTitleLabel->setMinimumWidth(r.width());
    statWriteBytesTitleLabel->setMinimumWidth(r.width());
    statWriteBytesRateTitleLabel->setMinimumWidth(r.width());

    setLayout(new QVBoxLayout());

    auto *layoutWidget = new QWidget(this);
    layoutWidget->setLayout(new QHBoxLayout());
    layoutWidget->layout()->setMargin(0);

    layoutWidget->layout()->addWidget(statReadBytesTitleLabel);
    (dynamic_cast<QHBoxLayout *>(layoutWidget->layout()))->addWidget(statReadBytesLabel, 1);

    layout()->addWidget(layoutWidget);

    layoutWidget = new QWidget(this);
    layoutWidget->setLayout(new QHBoxLayout());
    layoutWidget->layout()->setMargin(0);

    layoutWidget->layout()->addWidget(statReadBytesRateTitleLabel);
    (dynamic_cast<QHBoxLayout *>(layoutWidget->layout()))->addWidget(statReadBytesRateLabel, 1);

    layout()->addWidget(layoutWidget);

    layout()->addItem(new QSpacerItem(0,10));

    layoutWidget = new QWidget(this);
    layoutWidget->setLayout(new QHBoxLayout());
    layoutWidget->layout()->setMargin(0);

    layoutWidget->layout()->addWidget(statWriteBytesTitleLabel);
    (dynamic_cast<QHBoxLayout *>(layoutWidget->layout()))->addWidget(statWriteBytesLabel, 1);

    layout()->addWidget(layoutWidget);

    layoutWidget = new QWidget(this);
    layoutWidget->setLayout(new QHBoxLayout());
    layoutWidget->layout()->setMargin(0);

    layoutWidget->layout()->addWidget(statWriteBytesRateTitleLabel);
    (dynamic_cast<QHBoxLayout *>(layoutWidget->layout()))->addWidget(statWriteBytesRateLabel, 1);

    layout()->addWidget(layoutWidget);

    layout()->addItem(new QSpacerItem(0,10));

    layout()->addWidget(openFilesTitleLabel);
    layout()->addWidget(openFilesListWidget);
    layout()->addWidget(openFilesPushButton);

    connect(openFilesPushButton, SIGNAL(pressed()), this, SLOT(onOpenFilePressed()));
}

void DiskTab::setData(const SystemStatus &status,
                      const SystemStatus &prevStatus,
                      const Process &proc,
                      const Process &prevProc) {
    lastUpdate = std::chrono::high_resolution_clock::now();
    auto delta = std::chrono::high_resolution_clock::now() - lastUpdate;

    openFilesListWidget->clear();
    for (auto &f : proc.openFiles) {
        openFilesListWidget->addItem(f.c_str());
    }

    statReadBytesLabel->setText(std::to_string(proc.rchar).c_str());
    statWriteBytesLabel->setText(std::to_string(proc.wchar).c_str());

    auto rate = getRate(std::chrono::duration_cast<std::chrono::milliseconds>(delta).count() / 1000.0f,
                        proc.rchar,
                        prevProc.rchar);
    statReadBytesRateLabel->setText(std::to_string(rate).c_str());

    rate = getRate(std::chrono::duration_cast<std::chrono::milliseconds>(delta).count() / 1000.0f,
                   proc.wchar,
                   prevProc.wchar);
    statWriteBytesRateLabel->setText(std::to_string(rate).c_str());
}

void DiskTab::updateData(const SystemStatus &status,
                         const SystemStatus &prevStatus,
                         const Process &proc,
                         const Process &prevProc) {
    auto start = std::chrono::high_resolution_clock::now();
    auto delta = start - lastUpdate;
    lastUpdate = start;

    auto index = openFilesListWidget->currentIndex();
    openFilesListWidget->clear();
    for (auto &f : proc.openFiles) {
        openFilesListWidget->addItem(f.c_str());
    }
    openFilesListWidget->setCurrentIndex(index);

    statReadBytesLabel->setText(std::to_string(proc.rchar).c_str());
    statWriteBytesLabel->setText(std::to_string(proc.wchar).c_str());

    auto rate = getRate(std::chrono::duration_cast<std::chrono::milliseconds>(delta).count() / 1000.0f,
                        proc.rchar,
                        prevProc.rchar);
    statReadBytesRateLabel->setText(std::to_string(rate).c_str());

    rate = getRate(std::chrono::duration_cast<std::chrono::milliseconds>(delta).count() / 1000.0f,
                   proc.wchar,
                   prevProc.wchar);
    statWriteBytesRateLabel->setText(std::to_string(rate).c_str());
}

void DiskTab::clearData() {
    openFilesListWidget->clear();
    statReadBytesLabel->setText("");
    statReadBytesRateLabel->setText("");
    statWriteBytesLabel->setText("");
    statWriteBytesRateLabel->setText("");
}

void DiskTab::onOpenFilePressed() {
    auto item = openFilesListWidget->currentItem();
    if (item != nullptr) {
        auto f = item->text().toStdString();
    }
}
