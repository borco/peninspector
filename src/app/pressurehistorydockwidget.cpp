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

#include "pressurehistorydockwidget.h"

#include "penconfig.h"
#include "peninfo.h"
#include "pressurehistorymodel.h"

#include <QActionGroup>
#include <QChart>
#include <QChartView>
#include <QClipboard>
#include <QGraphicsLayout>
#include <QGuiApplication>
#include <QLineSeries>
#include <QMimeData>
#include <QPointF>
#include <QSettings>
#include <QSplitter>
#include <QTableView>
#include <QValueAxis>

namespace {
static const char* SettingsGroupKey { "PressureHistoryDock" };
static const char* SplitterStateKey {"splitterState"};
static const char* WindowSizeKey {"windowSize"};
}

PressureHistoryDockWidget::PressureHistoryDockWidget(PenConfig *penConfig, PenInfo *penInfo, QWidget *parent)
    : DockWidget("pressureHistoryDockWidget",
                 tr("Pressure History"),
                 parent)
    , m_config(penConfig)
    , m_info(penInfo)
    , m_chart(new QChart())
    , m_xAxis(new QValueAxis())
    , m_yAxis(new QValueAxis())
    , m_pressureHistoryModel(new PressureHistoryModel(penConfig, this))
    , m_windowSizeActionGroup(new QActionGroup(this))
{
    setupToolBarActions();
    setupWidgets();
    updateHistory();
}

PressureHistoryDockWidget::~PressureHistoryDockWidget()
{
    delete m_chart;
    delete m_pressureHistoryModel;
}

void PressureHistoryDockWidget::saveSettings() const
{
    QSettings settings;
    settings.beginGroup(SettingsGroupKey);
    settings.setValue(SplitterStateKey, m_splitter->saveState());
    settings.setValue(WindowSizeKey, m_pressureHistoryModel->windowSize());
    settings.endGroup();
}

void PressureHistoryDockWidget::loadSettings()
{
    QSettings settings;
    settings.beginGroup(SettingsGroupKey);
    m_splitter->restoreState(settings.value(SplitterStateKey).toByteArray());
    setHistoryWindowSize(settings.value(WindowSizeKey, 20).toInt());
    settings.endGroup();
}

void PressureHistoryDockWidget::setupToolBarActions()
{
    addToolBarSeparator();

    auto action = new QAction(this);
    action->setText(tr("Copy Chart"));
    connect(action, &QAction::triggered, this, &PressureHistoryDockWidget::copyChartToClipboard);
    addToolBarAction(action);

    //    action = new QAction(this);
    //    action->setText(tr("Save Chart"));
    //    connect(action, &QAction::triggered, this, &PressureHistoryDockWidget::saveChartToDisk);
    //    addToolBarAction(action);

    addToolBarSeparator();

    action = new QAction(this);
    action->setText(tr("Clear"));
    connect(action, &QAction::triggered, m_pressureHistoryModel, &PressureHistoryModel::clear);
    addToolBarAction(action);

    addToolBarSeparator();

    addToolBarLabel(tr("Window Size:"));

    QList<int> window_sizes = {10, 20, 40, 100, 200, 400};

    for (int window_size: window_sizes) {
        action = new QAction(this);
        action->setData(window_size);
        action->setText(window_size > 0 ? QString::number(window_size) : tr("All"));
        action->setCheckable(true);
        connect(action, &QAction::triggered, this, [this, window_size]() { m_pressureHistoryModel->setWindowSize(window_size); });
        m_windowSizeActionGroup->addAction(action);
        m_windowSizeActions << action;
        addToolBarAction(action);
    }

    setHistoryWindowSize(window_sizes.last());
}

void PressureHistoryDockWidget::setupWidgets()
{
    m_xAxis->setLabelFormat("%i");
    m_yAxis->setLabelFormat("%i");

    m_chart->layout()->setContentsMargins(0, 0, 0, 0);
    m_chart->setBackgroundRoundness(0);
    m_chart->addAxis(m_xAxis, Qt::AlignBottom);
    m_chart->addAxis(m_yAxis, Qt::AlignLeft);
    m_chart->legend()->hide();

    connect(m_config, &PenConfig::nameChanged, this, [this](){
        updateTitle();
    });
    updateTitle();

    connect(m_pressureHistoryModel, &PressureHistoryModel::modelReset, this, &PressureHistoryDockWidget::updateHistory);

    connect(m_pressureHistoryModel, &PressureHistoryModel::windowSizeChanged, this, &PressureHistoryDockWidget::updateHistory);

    connect(m_info, &PenInfo::pressureChanged, this, [this]() {
        m_pressureHistoryModel->addPressure(m_info->pressure());
    });

    m_splitter = new QSplitter(this);

    m_chartView = new QChartView(m_chart, this);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_splitter->addWidget(m_chartView);

    auto table_view = new QTableView(this);
    table_view->setFrameShape(QFrame::NoFrame);
    table_view->setModel(m_pressureHistoryModel);
    m_splitter->addWidget(table_view);

    m_splitter->setSizes(QList<int>() << 1 << 0);

    setWidget(m_splitter);
}

void PressureHistoryDockWidget::updateHistory()
{
    auto series = new QLineSeries();

    for (int i = 0; i < m_pressureHistoryModel->size(); ++i) {
        const auto pressure = m_pressureHistoryModel->at(i);
        series->append(QPointF(pressure.index, pressure.level));
        series->append(QPointF(pressure.index + 1, pressure.level));
    }

    m_chart->removeAllSeries();
    m_chart->addSeries(series);

    m_xAxis->setRange(m_pressureHistoryModel->first().index, m_pressureHistoryModel->last().index);
    series->attachAxis(m_xAxis);

    m_yAxis->setRange(0, m_config->pressureLevels());
    series->attachAxis(m_yAxis);
}

void PressureHistoryDockWidget::setHistoryWindowSize(int windowSize)
{
    for (const auto& action: m_windowSizeActions) {
        auto action_window_size = action->data().toInt();
        if (action_window_size == windowSize) {
            action->setChecked(true);
            m_pressureHistoryModel->setWindowSize(windowSize);
            return;
        }
    }

    const auto& action = m_windowSizeActions.last();
    action->setChecked(true);
    m_pressureHistoryModel->setWindowSize(action->data().toInt());
}

void PressureHistoryDockWidget::updateTitle()
{
    m_chart->setTitle(tr("<b>Pressure History | %1</b>").arg(m_config->name()));
}

void PressureHistoryDockWidget::copyChartToClipboard()
{
    auto clipboard = QGuiApplication::clipboard();
    auto data = new QMimeData;
    data->setImageData(m_chartView->grab().toImage());
    clipboard->setMimeData(data);
}

//void PressureHistoryDockWidget::saveChartToDisk()
//{
//}
