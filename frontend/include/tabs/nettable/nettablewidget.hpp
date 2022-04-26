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

#ifndef WEASEL_NETTABLEWIDGET_HPP
#define WEASEL_NETTABLEWIDGET_HPP

#include <QTableWidget>
#include <QAction>

#include "weasel/types.hpp"
#include "weasel/snapshot.hpp"

class NetTableWidget : public QWidget {
Q_OBJECT
public:
    NetTableWidget();

signals:

    void terminateProcess(Pid_t pid);

    void viewProcess(Pid_t pid);

public slots:

    void onSnapshot(const Snapshot &snapshot);

private slots:

    void customContextMenu(const QPoint &pos);

private:
    enum ActionType {
        TERMINATE,
        VIEW_PROCESS,
        COPY
    };

    class Action : public QAction {
    public:
        Action(const QString &text, ActionType type)
                : QAction(), type(type) {
            setText(text);
        }

        ActionType type;
    };

    QTableWidget *tableWidget;

    std::vector<Pid_t> processes;
};

#endif //WEASEL_NETTABLEWIDGET_HPP
