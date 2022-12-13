/*
    Copyright 2022 by Ioan Calin Borcoman <iborco@gmail.com>

    This file is part of PenInspector.

    PenInspector is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or (at your option)
    any later version.

    PenInspector is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
    more details.

    You should have received a copy of the GNU General Public License along with
    PenInspector. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include "dockwidget.h"

class PenConfig;
class PenInfo;
class PressureHistoryModel;

class QAbstractAxis;
class QChart;
class QChartView;
class QSplitter;

class PressureHistoryDockWidget : public DockWidget
{
public:
    PressureHistoryDockWidget(PenConfig *penConfig, PenInfo *penInfo, QWidget* parent = nullptr);
    ~PressureHistoryDockWidget() override;

    void saveSettings() const override;
    void loadSettings() override;

private:
    void setupToolBarActions();
    void setupWidgets();
    void updateHistory();
    void copyChartToClipboard();
    void saveChartToDisk();

    PenConfig* m_config {nullptr};
    PenInfo* m_info {nullptr};

    QChart* m_chart {nullptr};
    QChartView* m_chartView {nullptr};
    QAbstractAxis* m_xAxis {nullptr};
    QAbstractAxis* m_yAxis {nullptr};

    PressureHistoryModel* m_pressureHistoryModel {nullptr};
    QSplitter* m_splitter {nullptr};
};

