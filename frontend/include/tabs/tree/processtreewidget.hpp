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

#include <set>

#include <QWidget>
#include <QTreeView>
#include <QStandardItemModel>
#include <QAction>

#include "weasel/process.hpp"
#include "weasel/snapshot.hpp"

#include "processtreeitem.hpp"

class ProcessTreeWidget : public QWidget {
Q_OBJECT
public:
    enum ActionType {
        SIGNAL_PROCESS,
        SIGNAL_THREAD,
        OPEN_PROCESS_DIALOG,
        EXPAND_ALL,
        COLLAPSE_ALL,
        NONE
    };

    class ProcessAction : public QAction {
    public:
        ProcessAction() = default;

        ProcessAction(ActionType type, Pid_t pid, const QString &text)
                : type(type), pid(pid) {
            setText(text);
        }

        ProcessAction(ActionType type, const QString &text)
                : type(type) {
            setText(text);
        }

        ProcessAction(ActionType type, Pid_t pid, Thread::Signal signal, const QString &text)
                : type(type), pid(pid), signal(signal) {
            setText(text);
        }

        const ActionType &getType() const { return type; }

        const Pid_t &getPid() const { return pid; }

        const Thread::Signal &getSignal() const { return signal; }

        void setType(ActionType t) { type = t; }

        void setPid(Pid_t v) { pid = v; }

        void setSignal(Thread::Signal s) { signal = s; }

    private:
        ActionType type = NONE;
        Pid_t pid = 0;
        Thread::Signal signal = Thread::SIGNAL_SIGHUP;
    };

    explicit ProcessTreeWidget(QWidget *parent = nullptr);

    ~ProcessTreeWidget() override;

    void selectProcess(Pid_t pid);

public slots:

    void onSnapshot(const Snapshot &snapshot);

signals:

    void processSignalRequested(Pid_t pid, Thread::Signal signal);

    void threadSignalRequested(Pid_t tid, Thread::Signal signal);

    void processPriorityChangeRequested(Pid_t pid, int priority);

    void threadPriorityChangeRequested(Pid_t tid, int priority);

    void processPropertiesRequested(Pid_t pid);

private slots:

    void clicked();

    void doubleClicked(const QModelIndex &index);

    void customContextMenu(const QPoint &pos);

private:
    QStandardItemModel model;

    QTreeView *treeView;

    std::map<Pid_t, ProcessTreeItem *> items;
};

#endif //WEASEL_PROCESSTREEWIDGET_HPP
