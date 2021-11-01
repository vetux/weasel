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

#include "dialog/tab/disktab.hpp"

#include <QVBoxLayout>
#include <QLineEdit>
#include <QSpacerItem>

#include "byteformatting.hpp"

DiskTab::DiskTab(QWidget *parent)
        : QWidget(parent) {
    openFilesTitleLabel = new QLabel(this);
    openFilesListWidget = new QListWidget(this);

    statReadBytesTitleLabel = new QLabel(this);
    statReadBytesLabel = new QLineEdit(this);

    statReadBytesRateTitleLabel = new QLabel(this);
    statReadBytesRateLabel = new QLineEdit(this);

    statWriteBytesTitleLabel = new QLabel(this);
    statWriteBytesLabel = new QLineEdit(this);

    statWriteBytesRateTitleLabel = new QLabel(this);
    statWriteBytesRateLabel = new QLineEdit(this);

    openFilesTitleLabel->setText("Open Files");

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

    layout()->addItem(new QSpacerItem(0, 10));

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

    layout()->addItem(new QSpacerItem(0, 10));

    layout()->addWidget(openFilesTitleLabel);
    layout()->addWidget(openFilesListWidget);
}

void DiskTab::setOpenFiles(const std::vector<std::string> &files) {
    openFilesListWidget->clear();
    for (auto &f: files) {
        openFilesListWidget->addItem(f.c_str());
    }
}

void DiskTab::setReadBytes(const std::string& bytes) {
    statReadBytesLabel->setText(bytes.c_str());
}

void DiskTab::setWriteBytes(const std::string& bytes) {
    statWriteBytesLabel->setText(bytes.c_str());
}

void DiskTab::setReadRate(const std::string& rate) {
    statReadBytesRateLabel->setText(rate.c_str());
}

void DiskTab::setWriteRate(const std::string& rate) {
    statWriteBytesRateLabel->setText(rate.c_str());
}

void DiskTab::onOpenFilePressed() {
    auto item = openFilesListWidget->currentItem();
    if (item != nullptr) {
        auto f = item->text().toStdString();
    }
}
