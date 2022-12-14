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

#include "pressurehistogramdockwidget.h"

#include "penconfig.h"
#include "peninfo.h"
#include "pressurehistogrammodel.h"

#include <QActionGroup>
#include <QChart>
#include <QChartView>
#include <QClipboard>
#include <QGraphicsLayout>
#include <QGuiApplication>
#include <QLineSeries>
#include <QMimeData>
#include <QSettings>
#include <QSplitter>
#include <QTableView>
#include <QValueAxis>
#include <set>

namespace {
static const char* SettingsGroupKey { "PressureHistogramDock" };
static const char* SplitterStateKey {"splitterState"};
static const char* WindowSizeKey {"windowSize"};
}

PressureHistogramDockWidget::PressureHistogramDockWidget(PenConfig* penConfig, PenInfo *penInfo, QWidget *parent)
    : DockWidget("pressureHistogramDockWidget",
                 tr("Pressure Histogram"),
                 parent)
    , m_config(penConfig)
    , m_info(penInfo)
    , m_chart(new QChart())
    , m_xAxis(new QValueAxis())
    , m_yAxis(new QValueAxis())
    , m_pressureHistogramModel(new PressureHistogramModel(penConfig, this))
    , m_windowSizeActionGroup(new QActionGroup(this))
{
    setupToolBarActions();
    setupWidgets();
    updateHistogram();
}

PressureHistogramDockWidget::~PressureHistogramDockWidget()
{
    delete m_chart;
    delete m_pressureHistogramModel;
}

void PressureHistogramDockWidget::updateHistogram()
{
    const auto levels = m_config->pressureLevels();

    auto series = new QLineSeries();

    int data[levels];
    std::memset(data, 0, sizeof data);

    for (int i = 0; i < m_pressureHistogramModel->histogramSize(); ++i) {
        const auto pressure = m_pressureHistogramModel->at(i);
        int index = pressure.value * levels;
        data[index] += pressure.count;
    }

    int max_count {0};
    for (int i = 0; i < levels; ++i) {
        series->append(QPoint(i, data[i]));
        max_count = qMax(max_count, data[i]);
    }

    m_chart->removeAllSeries();
    m_chart->addSeries(series);

    m_xAxis->setRange(0, levels);
    series->attachAxis(m_xAxis);

    m_yAxis->setRange(0, max_count);
    series->attachAxis(m_yAxis);
}

void PressureHistogramDockWidget::setHistogramWindowSize(int windowSize)
{
    for (const auto& action: m_windowSizeActions) {
        auto action_window_size = action->data().toInt();
        if (action_window_size == windowSize) {
            action->setChecked(true);
            m_pressureHistogramModel->setWindowSize(windowSize);
            return;
        }
    }

    const auto& action = m_windowSizeActions.last();
    action->setChecked(true);
    m_pressureHistogramModel->setWindowSize(action->data().toInt());
}

void PressureHistogramDockWidget::updateTitle()
{
    m_chart->setTitle(tr("<center><b>Pressure Histogram | %1</b><br>pressure levels: %2, detected levels: %3, in window: %4</center>")
                      .arg(m_config->name())
                      .arg(m_config->pressureLevels())
                      .arg(m_pressureHistogramModel->totalLevels())
                      .arg(m_pressureHistogramModel->histogramSize())
                    );}

void PressureHistogramDockWidget::copyChartToClipboard()
{
    auto clipboard = QGuiApplication::clipboard();
    auto data = new QMimeData;
    data->setImageData(m_chartView->grab().toImage());
    clipboard->setMimeData(data);
}

//void PressureHistogramDockWidget::saveChartToDisk()
//{
//}

void PressureHistogramDockWidget::saveSettings() const
{
    QSettings settings;
    settings.beginGroup(SettingsGroupKey);
    settings.setValue(SplitterStateKey, m_splitter->saveState());
    settings.setValue(WindowSizeKey, m_pressureHistogramModel->windowSize());
    settings.endGroup();
}

void PressureHistogramDockWidget::loadSettings()
{
    QSettings settings;
    settings.beginGroup(SettingsGroupKey);
    m_splitter->restoreState(settings.value(SplitterStateKey).toByteArray());
    setHistogramWindowSize(settings.value(WindowSizeKey, 20).toInt());
    settings.endGroup();
}

void PressureHistogramDockWidget::setupToolBarActions()
{
    addToolBarSeparator();

    auto action = new QAction(this);
    action->setText(tr("Copy Chart"));
    connect(action, &QAction::triggered, this, &PressureHistogramDockWidget::copyChartToClipboard);
    addToolBarAction(action);

    addToolBarSeparator();

    action = new QAction(this);
    action->setText(tr("Clear"));
    connect(action, &QAction::triggered, m_pressureHistogramModel, &PressureHistogramModel::clear);
    addToolBarAction(action);

    addToolBarSeparator();

    addToolBarLabel(tr("Window Size:"));

    QList<int> window_sizes = {10, 20, 40, 100, 200, -1};

    for (int window_size: window_sizes) {
        action = new QAction(this);
        action->setData(window_size);
        action->setText(window_size > 0 ? QString::number(window_size) : tr("All"));
        action->setCheckable(true);
        connect(action, &QAction::triggered, this, [this, window_size]() { m_pressureHistogramModel->setWindowSize(window_size); });
        m_windowSizeActionGroup->addAction(action);
        m_windowSizeActions << action;
        addToolBarAction(action);
    }

    setHistogramWindowSize(window_sizes.last());
}

void PressureHistogramDockWidget::setupWidgets()
{
    m_xAxis->setLabelFormat("%i");
    m_yAxis->setVisible(false);

    m_chart->layout()->setContentsMargins(0, 0, 0, 0);
    m_chart->setBackgroundRoundness(0);
    m_chart->addAxis(m_xAxis, Qt::AlignBottom);
    m_chart->addAxis(m_yAxis, Qt::AlignLeft);
    m_chart->legend()->hide();

    connect(m_config, &PenConfig::nameChanged, this, &PressureHistogramDockWidget::updateTitle);

    connect(m_config, &PenConfig::pressureLevelsChanged, this, [this]() {
        updateHistogram();
        updateTitle();
    });

    connect(m_pressureHistogramModel, &PressureHistogramModel::modelReset, this, [this]() {
        updateHistogram();
        updateTitle();
    });

    connect(m_info, &PenInfo::pressureChanged, this, [this]() {
        m_pressureHistogramModel->addPressure(m_info->pressure());
        updateHistogram();
        updateTitle();
    });

    m_splitter = new QSplitter(this);

    m_chartView = new QChartView(m_chart, this);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_splitter->addWidget(m_chartView);

    auto table_view = new QTableView(this);
    table_view->setFrameShape(QFrame::NoFrame);
    table_view->setModel(m_pressureHistogramModel);
    m_splitter->addWidget(table_view);

    m_splitter->setSizes(QList<int>() << 1 << 0);

    setWidget(m_splitter);
}
