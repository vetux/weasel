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

#ifndef WEASEL_DISKTAB_HPP
#define WEASEL_DISKTAB_HPP

#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <chrono>

#include "system/process.hpp"
#include "system/systemstatus.hpp"

class DiskTab : public QWidget {
Q_OBJECT
public:
    explicit DiskTab(QWidget *parent = nullptr);

public slots:

    void setData(const SystemStatus &status,
                 const SystemStatus &prevStatus,
                 const Process &proc,
                 const Process &prevProc);

    void updateData(const SystemStatus &status,
                    const SystemStatus &prevStatus,
                    const Process &proc,
                    const Process &prevProc);

    void clearData();

private slots:

    void onOpenFilePressed();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> lastUpdate;
    
    QLabel *openFilesTitleLabel;
    QListWidget *openFilesListWidget;
    QPushButton *openFilesPushButton;

    QLabel *statReadBytesTitleLabel;
    QLineEdit *statReadBytesLabel;

    QLabel *statReadBytesRateTitleLabel;
    QLineEdit *statReadBytesRateLabel;

    QLabel *statWriteBytesTitleLabel;
    QLineEdit *statWriteBytesLabel;

    QLabel *statWriteBytesRateTitleLabel;
    QLineEdit *statWriteBytesRateLabel;
};

#endif //WEASEL_DISKTAB_HPP
