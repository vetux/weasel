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

#ifndef WEASEL_THREADSTAB_HPP
#define WEASEL_THREADSTAB_HPP

#include <QWidget>

#include "system/process.hpp"
#include "system/systemstatus.hpp"

class ThreadsTab : public QWidget {
Q_OBJECT
public:
    explicit ThreadsTab(QWidget *parent = nullptr);

public slots:

    void setData(const SystemStatus &status,
                 const SystemStatus &prevStatus,
                 const Process &proc,
                 const Process &prevProc);

    void clearData();
};

#endif //WEASEL_THREADSTAB_HPP
