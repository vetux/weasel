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

#include "weasel/snapshot.hpp"

#include "tab/generaltab.hpp"
#include "tab/threadstab.hpp"
#include "tab/disktab.hpp"
#include "tab/networktab.hpp"

class ProcessPropertiesDialog : public QDialog {
Q_OBJECT
public:
    ProcessPropertiesDialog(QWidget *parent, Pid_t pid);

    Pid_t getPid() const;

signals:

    void openPath(const QString &path);

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:

    void onSnapshot(const Snapshot &snapshot);

private:
    Pid_t pid;

    QTabWidget *tabWidget;

    GeneralTab *tabGeneral;
    ThreadsTab *tabThreads;
    DiskTab *tabDisk;
    NetworkTab *tabNetwork;

    bool processDead = false;
};

#endif //WEASEL_PROCESSPROPERTIESDIALOG_HPP
