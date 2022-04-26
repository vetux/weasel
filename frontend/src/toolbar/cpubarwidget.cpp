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

#include "toolbar/cpubarwidget.hpp"

#include <QHBoxLayout>

CpuBarWidget::CpuBarWidget(QWidget *parent) : QWidget(parent) {
    setLayout(new QVBoxLayout());
    layout()->setMargin(0);
}

void CpuBarWidget::setCpus(const std::vector<CpuState::Core> &cores) {
    int maxLine = cores.size() / 2;
    if (maxLine < 1)
        maxLine = 1;

    if (cpuBars.size() > cores.size()) {
        auto *p = cpuBars.at(cpuBars.size() - 1);
        delete p;
        cpuBars.pop_back();
        widgets.resize(cores.size() / maxLine);
    }

    if (cpuBars.size() < cores.size()) {
        auto *layout = new QHBoxLayout();
        layout->setMargin(0);

        int count = 0;

        while (cpuBars.size() < cores.size()) {
            auto *progressBar = new QProgressBar();
            auto *label = new QLabel;
            auto *plabel = new QLabel;

            progressBar->setRange(0, 100);

            label->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
            label->setContentsMargins(0, 0, 6, 0);

            plabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            plabel->setContentsMargins(6, 0, 0, 0);

            progressBar->setLayout(new QHBoxLayout());
            progressBar->layout()->addWidget(plabel);
            progressBar->layout()->addWidget(label);
            progressBar->layout()->setMargin(0);
            progressBar->setTextVisible(false);

            count++;

            if (count > maxLine) {
                count = 0;
                auto *w = new QWidget();
                w->setLayout(layout);
                this->layout()->addWidget(w);
                layout = new QHBoxLayout();
                layout->setMargin(0);
                widgets.emplace_back(w);
            }

            layout->addWidget(progressBar);

            cpuBars.emplace_back(progressBar);
            cpuLabels.emplace_back(label);
            percentageLabels.emplace_back(plabel);
        }

        auto *w = new QWidget();
        w->setLayout(layout);
        this->layout()->addWidget(w);
        widgets.emplace_back(w);
    }

    for (int i = 0; i < cores.size(); i++) {
        auto &core = cores.at(i);

        auto *bar = cpuBars.at(i);
        auto *label = cpuLabels.at(i);
        auto *plabel = percentageLabels.at(i);

        auto percentage = static_cast<int>(100 * core.load);

        bar->setValue(percentage);
        label->setText(("Cpu " + std::to_string(i)).c_str());
        plabel->setText((std::to_string(percentage) + "%").c_str());
    }
}