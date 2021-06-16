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

#include "frontend/qt/widget/cpubarwidget.hpp"

#include <QHBoxLayout>

static int getCpuPercentage(const SystemStatus::Core &core, const SystemStatus::Core &prev) {
    //https://stackoverflow.com/a/23376195
    //https://unix.stackexchange.com/a/27079
    auto prevIdle = prev.idle + prev.iowait;
    auto idle = core.idle + core.iowait;

    auto prevNonIdle = prev.user + prev.nice + prev.system + prev.irq + prev.softirq + prev.steal;
    auto nonIdle = core.user + core.nice + core.system + core.irq + core.softirq + core.steal;

    auto prevTotal = prevIdle + prevNonIdle;
    auto total = idle + nonIdle;

    auto totald = total - prevTotal;
    auto idled = idle - prevIdle;

    auto percentage = 100 * (totald - idled) / totald;

    return (int) percentage;
}

CpuBarWidget::CpuBarWidget(QWidget *parent) : QWidget(parent) {
    totalCpuBar = new QProgressBar();
    totalCpuLabel = new QLabel();

    totalCpuBar->setRange(0, 100);

    totalCpuLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    totalCpuLabel->setText("Total");
    totalCpuLabel->setContentsMargins(0, 0, 6, 0);

    totalCpuBar->setLayout(new QHBoxLayout());
    totalCpuBar->layout()->addWidget(totalCpuLabel);
    totalCpuBar->layout()->setMargin(0);

    setLayout(new QHBoxLayout());
    layout()->addWidget(totalCpuBar);
}

void CpuBarWidget::setTotalCpu(const SystemStatus::Core &core, const SystemStatus::Core &prev) {
    totalCpuBar->setValue((int) getCpuPercentage(core, prev));
}

void CpuBarWidget::setCpus(const std::vector<SystemStatus::Core> &cores,
                           const std::vector<SystemStatus::Core> &prevCores) {
    while (cpuBars.size() > cores.size()) {
        auto *p = cpuBars.at(cpuBars.size() - 1);
        delete p;
        cpuBars.pop_back();
    }

    while (cpuBars.size() < cores.size()) {
        auto *progressBar = new QProgressBar();
        auto *label = new QLabel;

        progressBar->setRange(0, 100);

        label->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
        label->setContentsMargins(0, 0, 6, 0);

        progressBar->setLayout(new QHBoxLayout());
        progressBar->layout()->addWidget(label);
        progressBar->layout()->setMargin(0);

        layout()->addWidget(progressBar);

        cpuBars.emplace_back(progressBar);
        cpuLabels.emplace_back(label);
    }

    if (!prevCores.empty()) {
        for (int i = 0; i < cores.size(); i++) {
            auto &core = cores.at(i);
            auto &prev = prevCores.at(i);
            auto *bar = cpuBars.at(i);
            auto *label = cpuLabels.at(i);
            bar->setValue(getCpuPercentage(core, prev));
            label->setText(("Cpu " + std::to_string(i)).c_str());
        }
    }
}