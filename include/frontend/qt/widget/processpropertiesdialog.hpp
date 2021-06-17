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

#ifndef WEASEL_PROCESSPROPERTIESDIALOG_HPP
#define WEASEL_PROCESSPROPERTIESDIALOG_HPP

#include <QDialog>
#include <QTabWidget>

#include <utility>

#include "system/process.hpp"
#include "system/systemstatus.hpp"

#include "frontend/qt/widget/tab/generaltab.hpp"
#include "frontend/qt/widget/tab/threadstab.hpp"
#include "frontend/qt/widget/tab/disktab.hpp"
#include "frontend/qt/widget/tab/networktab.hpp"

class ProcessPropertiesDialog : public QDialog {
Q_OBJECT
public:
    ProcessPropertiesDialog(QWidget *parent, Process process);

public slots:
    void onRefresh(const SystemStatus &status,
                   const SystemStatus &prevStatus,
                   const std::map<Pid_t, Process> &processes,
                   const std::map<Pid_t, Process> &prevProc);

private:
    Process process;

    QTabWidget *tabWidget;

    GeneralTab *tabGeneral;
    ThreadsTab *tabThreads;
    DiskTab *tabDisk;
    NetworkTab *tabNetwork;
};

#endif //WEASEL_PROCESSPROPERTIESDIALOG_HPP
