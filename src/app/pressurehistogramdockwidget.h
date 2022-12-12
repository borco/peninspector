// Copyright 2022 by Ioan Calin Borcoman <iborco@gmail.com>

#pragma once

#include "dockwidget.h"

class PenConfig;
class PenInfo;
class PressureHistogramModel;

class QAbstractAxis;
class QChart;
class QChartView;
class QSplitter;

class PressureHistogramDockWidget : public DockWidget
{
public:
    explicit PressureHistogramDockWidget(PenConfig *penConfig, PenInfo *penInfo, QWidget* parent = nullptr);
    ~PressureHistogramDockWidget() override;

    void saveSettings() const override;
    void loadSettings() override;

private:
    void updateHistogram();
    void copyChartToClipboard();

    PenConfig* m_config {nullptr};
    PenInfo* m_info {nullptr};

    QChart* m_chart {nullptr};
    QChartView* m_chartView {nullptr};
    QAbstractAxis* m_xAxis {nullptr};
    QAbstractAxis* m_yAxis {nullptr};

    PressureHistogramModel* m_pressureHistogramModel {nullptr};
    QSplitter* m_splitter {nullptr};
};
