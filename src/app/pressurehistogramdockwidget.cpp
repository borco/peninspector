#include "pressurehistogramdockwidget.h"

#include "penconfig.h"
#include "peninfo.h"
#include "pressurehistogrammodel.h"

#include <QChart>
#include <QChartView>
#include <QClipboard>
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
    , m_pressureHistogramModel(new PressureHistogramModel(this))
{
    addToolBarSeparator();

    auto action = new QAction(this);
    action->setText(tr("Copy Chart"));
    connect(action, &QAction::triggered, this, &PressureHistogramDockWidget::copyChartToClipboard);
    addToolBarAction(action);

    m_chart->addAxis(m_xAxis, Qt::AlignBottom);
    m_chart->addAxis(m_yAxis, Qt::AlignLeft);

    connect(m_config, &PenConfig::pressureLevelsChanged, this, &PressureHistogramDockWidget::updateHistogram);
    connect(m_info, &PenInfo::pressureChanged, this, [this]() {
        m_pressureHistogramModel->addPressure(m_info->pressure());
        updateHistogram();
    });

    m_splitter = new QSplitter(this);

    m_chartView = new QChartView(m_chart, this);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_splitter->addWidget(m_chartView);

    auto table_view = new QTableView(this);
    table_view->setModel(m_pressureHistogramModel);
    m_splitter->addWidget(table_view);

    setWidget(m_splitter);
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

        uint data[levels];
        std::memset(data, 0, sizeof data);

        for (int i = 0; i < m_pressureHistogramModel->size(); ++i) {
            const auto pressure = (*m_pressureHistogramModel)[i];
            uint index = pressure.value / PressureHistogramModel::PressureMultiplier * levels;
            data[index] += pressure.count;
        }

        uint max_count {0};
        for (int i = 0; i < levels; ++i) {
            series->append(QPoint(i, data[i]));
            max_count = qMax(max_count, data[i]);
        }

        series->setName(tr("Pressure (%1 values)").arg(m_pressureHistogramModel->size()));

        m_chart->removeAllSeries();
        m_chart->addSeries(series);

        m_xAxis->setRange(0, levels);
        series->attachAxis(m_xAxis);

        m_yAxis->setRange(0, max_count);
        series->attachAxis(m_yAxis);
}

void PressureHistogramDockWidget::copyChartToClipboard()
{
    auto clipboard = QGuiApplication::clipboard();
    auto data = new QMimeData;
    data->setImageData(m_chartView->grab().toImage());
    clipboard->setMimeData(data);
}

void PressureHistogramDockWidget::saveSettings() const
{
    QSettings settings;
    settings.beginGroup(SettingsGroupKey);
    settings.setValue(SplitterStateKey, m_splitter->saveState());
    settings.endGroup();
}

void PressureHistogramDockWidget::loadSettings()
{
    QSettings settings;
    settings.beginGroup(SettingsGroupKey);
    m_splitter->restoreState(settings.value(SplitterStateKey).toByteArray());
    settings.endGroup();
}
