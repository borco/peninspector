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

#include <QChart>
#include <QChartView>
#include <QGraphicsLayout>
#include <QSettings>
#include <QSplitter>
#include <QTableView>
#include <QValueAxis>

namespace {
static const char* SettingsGroupKey { "PressureHistoryDock" };
static const char* SplitterStateKey {"splitterState"};
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
{
    setupToolBarActions();
    setupWidgets();
    updateHistory();
}

PressureHistoryDockWidget::~PressureHistoryDockWidget()
{
    delete m_chart;
}

void PressureHistoryDockWidget::saveSettings() const
{
    QSettings settings;
    settings.beginGroup(SettingsGroupKey);
    settings.setValue(SplitterStateKey, m_splitter->saveState());
    settings.endGroup();
}

void PressureHistoryDockWidget::loadSettings()
{
    QSettings settings;
    settings.beginGroup(SettingsGroupKey);
    m_splitter->restoreState(settings.value(SplitterStateKey).toByteArray());
    settings.endGroup();
}

void PressureHistoryDockWidget::setupToolBarActions()
{
    addToolBarSeparator();

    auto action = new QAction(this);
    action->setText(tr("Copy Chart"));
    connect(action, &QAction::triggered, this, &PressureHistoryDockWidget::copyChartToClipboard);
    addToolBarAction(action);

    action = new QAction(this);
    action->setText(tr("Save Chart"));
    connect(action, &QAction::triggered, this, &PressureHistoryDockWidget::saveChartToDisk);
    addToolBarAction(action);
}

void PressureHistoryDockWidget::setupWidgets()
{
    m_chart->layout()->setContentsMargins(0, 0, 0, 0);
    m_chart->setBackgroundRoundness(0);
    m_chart->addAxis(m_xAxis, Qt::AlignBottom);
    m_chart->addAxis(m_yAxis, Qt::AlignLeft);

    connect(m_config, &PenConfig::pressureLevelsChanged, this, &PressureHistoryDockWidget::updateHistory);
    connect(m_info, &PenInfo::pressureChanged, this, [this]() {
//        m_pressureHistogramModel->addPressure(m_info->pressure());
        updateHistory();
    });

    m_splitter = new QSplitter(this);

    m_chartView = new QChartView(m_chart, this);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_splitter->addWidget(m_chartView);

    auto table_view = new QTableView(this);
    table_view->setFrameShape(QFrame::NoFrame);
    m_splitter->addWidget(table_view);

    setWidget(m_splitter);
}

void PressureHistoryDockWidget::updateHistory()
{
}

void PressureHistoryDockWidget::copyChartToClipboard()
{
}

void PressureHistoryDockWidget::saveChartToDisk()
{
}
