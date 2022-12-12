#pragma once

#include "dockwidget.h"

class PenConfig;
class PenInspector;

class QAbstractAxis;
class QChart;
class QSplitter;
class QStringListModel;

class PressureHistogramDockWidget : public DockWidget
{
public:
    explicit PressureHistogramDockWidget(PenConfig *penConfig, PenInspector* penInspector, QWidget* parent = nullptr);
    ~PressureHistogramDockWidget() override;

    void saveSettings() const override;
    void loadSettings() override;

private:
    void updateHistogram();

    PenConfig* m_config {nullptr};
    PenInspector* m_inspector {nullptr};

    QChart* m_chart {nullptr};
    QAbstractAxis* m_xAxis {nullptr};
    QAbstractAxis* m_yAxis {nullptr};

    QStringListModel* m_valuesModel {nullptr};
    QSplitter* m_splitter {nullptr};
};

