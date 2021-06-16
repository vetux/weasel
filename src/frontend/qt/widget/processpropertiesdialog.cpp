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

#include "frontend/qt/widget/processpropertiesdialog.hpp"

#include <QVBoxLayout>


ProcessPropertiesDialog::ProcessPropertiesDialog(QWidget *parent, Process process)
        : QDialog(parent), process(std::move(process)) {
    setWindowTitle(("Properties " + std::to_string(process.pid)).c_str());

    tabWidget = new QTabWidget(this);
    tabGeneral = new QWidget(tabWidget);
    tabThreads = new QWidget(tabWidget);
    tabWidget->addTab(tabGeneral, "General");
    tabWidget->addTab(tabThreads, "Threads");

    setLayout(new QVBoxLayout());
    layout()->addWidget(tabWidget);

    layout()->setMargin(6);
}