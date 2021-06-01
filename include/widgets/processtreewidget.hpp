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

#ifndef WEASEL_PROCESSTREEWIDGET_HPP
#define WEASEL_PROCESSTREEWIDGET_HPP

#include <QWidget>

#include "sched/process.hpp"

class ProcessTreeWidget : public QWidget {
Q_OBJECT
signals:

    void processSignalRequested(Process *proc, Signal signal);

    void threadSignalRequested(Thread *thread, Signal signal);

public:
    ProcessTreeWidget();

    ~ProcessTreeWidget() override;

    void setProcesses(const std::vector<Process *> &processes);
};

#endif //WEASEL_PROCESSTREEWIDGET_HPP
