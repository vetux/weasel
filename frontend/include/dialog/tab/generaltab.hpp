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

#ifndef WEASEL_GENERALTAB_HPP
#define WEASEL_GENERALTAB_HPP

#include <qt5/QtWidgets/QWidget>

#include <qt5/QtWidgets/QLineEdit>
#include <qt5/QtWidgets/QLabel>
#include <qt5/QtWidgets/QPushButton>
#include <qt5/QtWidgets/QListWidget>

#include "weasel/process.hpp"

class GeneralTab : public QWidget {
Q_OBJECT
public:
    explicit GeneralTab(QWidget *parent = nullptr);

public slots:

    void setName(const std::string &name);

    void setPid(const std::string &pid);

    void setUserId(const std::string &uid);

    void setUserName(const std::string &name);

    void setCommandLine(const std::string &commandLine);

    void setExecutablePath(const std::string &path);

    void setRootDirectory(const std::string &path);

    void setEnvironment(const std::vector<std::string> &environ);

private:
    QLabel *processTitleLabel;
    QLineEdit *processPidLabel;
    QLineEdit *processNameLabel;

    QLabel *userTitleLabel;
    QLineEdit *userIdLabel;
    QLineEdit *userNameLabel;

    QLabel *commandLineTitleLabel;
    QLineEdit *commandLineLabel;

    QLabel *executablePathTitleLabel;
    QLineEdit *executablePathLabel;
    QPushButton *executablePathOpenPushButton;

    QLabel *rootDirTitleLabel;
    QLineEdit *rootDirLabel;
    QPushButton *rootDirOpenPushButton;

    QLabel *environTitleLabel;
    QListWidget *environListWidget;
};

#endif //WEASEL_GENERALTAB_HPP
