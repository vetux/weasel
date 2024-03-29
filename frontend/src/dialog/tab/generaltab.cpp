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

#include "dialog/tab/generaltab.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>

#include "filebrowser.hpp"
#include "strutil.hpp"

GeneralTab::GeneralTab(QWidget *parent)
        : QWidget(parent) {
    setLayout(new QVBoxLayout());

    processTitleLabel = new QLabel(this);
    processPidLabel = new QLineEdit(this);
    processNameLabel = new QLineEdit(this);

    userTitleLabel = new QLabel(this);
    userIdLabel = new QLineEdit(this);
    userNameLabel = new QLineEdit(this);

    commandLineTitleLabel = new QLabel(this);
    commandLineLabel = new QLineEdit(this);

    executablePathTitleLabel = new QLabel(this);
    executablePathLabel = new QLineEdit(this);
    executablePathOpenPushButton = new QPushButton(this);

    rootDirTitleLabel = new QLabel(this);
    rootDirLabel = new QLineEdit(this);
    rootDirOpenPushButton = new QPushButton(this);

    environTitleLabel = new QLabel(this);
    environListWidget = new QListWidget(this);

    processPidLabel->setReadOnly(true);
    processNameLabel->setReadOnly(true);
    userIdLabel->setReadOnly(true);
    userNameLabel->setReadOnly(true);
    commandLineLabel->setReadOnly(true);
    executablePathLabel->setReadOnly(true);
    rootDirLabel->setReadOnly(true);

    processTitleLabel->setText("Process ID / Name");
    userTitleLabel->setText("User ID / Name");
    commandLineTitleLabel->setText("Commandline");
    executablePathTitleLabel->setText("Executable");
    rootDirTitleLabel->setText("Root Directory");
    environTitleLabel->setText("Environment");

    executablePathOpenPushButton->setText("Open File Location");
    rootDirOpenPushButton->setText("Open File Location");

    auto *layoutWidget = new QWidget(this);
    layoutWidget->setLayout(new QVBoxLayout());

    auto *subLayoutWidget = new QWidget(this);
    subLayoutWidget->setLayout(new QHBoxLayout());
    subLayoutWidget->layout()->setMargin(0);

    subLayoutWidget->layout()->addWidget(processPidLabel);
    subLayoutWidget->layout()->addWidget(processNameLabel);

    layoutWidget->layout()->addWidget(processTitleLabel);
    layoutWidget->layout()->addWidget(subLayoutWidget);

    layout()->addWidget(layoutWidget);

    layoutWidget = new QWidget(this);
    layoutWidget->setLayout(new QVBoxLayout());

    subLayoutWidget = new QWidget(this);
    subLayoutWidget->setLayout(new QHBoxLayout());
    subLayoutWidget->layout()->setMargin(0);

    subLayoutWidget->layout()->addWidget(userIdLabel);
    subLayoutWidget->layout()->addWidget(userNameLabel);

    layoutWidget->layout()->addWidget(userTitleLabel);
    layoutWidget->layout()->addWidget(subLayoutWidget);

    layout()->addWidget(layoutWidget);

    layoutWidget = new QWidget(this);
    layoutWidget->setLayout(new QVBoxLayout());

    layoutWidget->layout()->addWidget(commandLineTitleLabel);
    layoutWidget->layout()->addWidget(commandLineLabel);

    layout()->addWidget(layoutWidget);

    layoutWidget = new QWidget(this);
    layoutWidget->setLayout(new QVBoxLayout());

    subLayoutWidget = new QWidget(this);
    subLayoutWidget->setLayout(new QHBoxLayout());
    subLayoutWidget->layout()->setMargin(0);

    subLayoutWidget->layout()->addWidget(executablePathLabel);
    subLayoutWidget->layout()->addWidget(executablePathOpenPushButton);

    layoutWidget->layout()->addWidget(executablePathTitleLabel);
    layoutWidget->layout()->addWidget(subLayoutWidget);

    layout()->addWidget(layoutWidget);

    layoutWidget = new QWidget(this);
    layoutWidget->setLayout(new QVBoxLayout());

    subLayoutWidget = new QWidget(this);
    subLayoutWidget->setLayout(new QHBoxLayout());
    subLayoutWidget->layout()->setMargin(0);

    subLayoutWidget->layout()->addWidget(rootDirLabel);
    subLayoutWidget->layout()->addWidget(rootDirOpenPushButton);

    layoutWidget->layout()->addWidget(rootDirTitleLabel);
    layoutWidget->layout()->addWidget(subLayoutWidget);

    layout()->addWidget(layoutWidget);

    layoutWidget = new QWidget(this);
    layoutWidget->setLayout(new QVBoxLayout());

    layoutWidget->layout()->addWidget(environTitleLabel);
    layoutWidget->layout()->addWidget(environListWidget);

    layout()->addWidget(layoutWidget);

    connect(executablePathOpenPushButton, SIGNAL(clicked(bool)), this, SLOT(onExecutablePathOpenClicked()));
    connect(rootDirOpenPushButton, SIGNAL(clicked(bool)), this, SLOT(onRootDirOpenClicked()));
}

void GeneralTab::setName(const std::string &name) {
    processNameLabel->setText(name.c_str());
}

void GeneralTab::setPid(const std::string& pid) {
    processPidLabel->setText(pid.c_str());
}

void GeneralTab::setUserId(const std::string& uid) {
    userIdLabel->setText(uid.c_str());
}

void GeneralTab::setUserName(const std::string &name) {
    userNameLabel->setText(name.c_str());
}

void GeneralTab::setCommandLine(const std::string &commandLine) {
    auto c = commandLine;
    replace(c, std::string("\0", 1), " ");
    commandLineLabel->setText(c.c_str());
}

void GeneralTab::setExecutablePath(const std::string &path) {
    executablePathLabel->setText(path.c_str());
}

void GeneralTab::setRootDirectory(const std::string &path) {
    rootDirLabel->setText(path.c_str());
}

void GeneralTab::setEnvironment(const std::vector<std::string> &environ) {
    environListWidget->clear();
    for (auto &e: environ) {
        environListWidget->addItem(e.c_str());
    }
}

void GeneralTab::onExecutablePathOpenClicked() {
    emit openPath(executablePathLabel->text());
}

void GeneralTab::onRootDirOpenClicked() {
    emit openPath(rootDirLabel->text());
}
