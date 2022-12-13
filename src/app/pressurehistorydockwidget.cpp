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

//    action = new QAction(this);
//    action->setText(tr("Save Chart"));
//    connect(action, &QAction::triggered, this, &PressureHistoryDockWidget::saveChartToDisk);
//    addToolBarAction(action);
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

    connect(m_config, &PenConfig::pressureLevelsChanged, this, &PressureHistoryDockWidget::updateHistory);
    connect(m_info, &PenInfo::pressureChanged, this, [this]() {
        m_pressureHistoryModel->addPressure(m_info->pressure());
        updateHistory();
    });

    m_splitter = new QSplitter(this);

    m_chartView = new QChartView(m_chart, this);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_splitter->addWidget(m_chartView);

    auto table_view = new QTableView(this);
    table_view->setFrameShape(QFrame::NoFrame);
    table_view->setModel(m_pressureHistoryModel);
    m_splitter->addWidget(table_view);

    setWidget(m_splitter);
}

void PressureHistoryDockWidget::updateHistory()
{
    auto series = new QLineSeries();

    for (int i = 0; i < m_pressureHistoryModel->size(); ++i) {
        const auto pressure = m_pressureHistoryModel->at(i);
        series->append(QPointF(pressure.index, pressure.level));
    }

    m_chart->removeAllSeries();
    m_chart->addSeries(series);

    if (m_pressureHistoryModel->size() > 0) {
        m_xAxis->setRange(m_pressureHistoryModel->at(0).index,
                          m_pressureHistoryModel->at(m_pressureHistoryModel->size() - 1).index);
    }
    series->attachAxis(m_xAxis);

    m_yAxis->setRange(0, m_config->pressureLevels());
    series->attachAxis(m_yAxis);
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
