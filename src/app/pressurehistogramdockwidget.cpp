#include "pressurehistogramdockwidget.h"

#include "penconfig.h"
#include "peninspector.h"

#include <QBarSeries>
#include <QBarSet>
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QSplitter>
#include <QStringListModel>
#include <QTableView>
#include <QValueAxis>
#include <qsettings.h>
#include <set>

namespace {
static const char* SettingsGroupKey { "PressureHistogramDock" };
static const char* SplitterStateKey {"splitterState"};
}

PressureHistogramDockWidget::PressureHistogramDockWidget(PenConfig* penConfig, PenInspector *penInspector, QWidget *parent)
    : DockWidget("pressureHistogramDockWidget",
                 tr("Pressure Histogram"),
                 parent)
    , m_config(penConfig)
    , m_inspector(penInspector)
    , m_chart(new QChart())
    , m_xAxis(new QValueAxis())
    , m_yAxis(new QValueAxis())
    , m_valuesModel(new QStringListModel(this))
{
    m_chart->addAxis(m_xAxis, Qt::AlignBottom);
    m_chart->addAxis(m_yAxis, Qt::AlignLeft);

    connect(penConfig, &PenConfig::pressureLevelsChanged, this, &PressureHistogramDockWidget::updateHistogram);
    connect(penInspector, &PenInspector::pressureChanged, this, &PressureHistogramDockWidget::updateHistogram);

    m_splitter = new QSplitter(this);
    auto char_view = new QChartView(m_chart, this);
    char_view->setRenderHint(QPainter::Antialiasing);
    m_splitter->addWidget(char_view);

    auto table_view = new QTableView(this);
    table_view->setModel(m_valuesModel);
    m_splitter->addWidget(table_view);

    setWidget(m_splitter);
    updateHistogram();
}

PressureHistogramDockWidget::~PressureHistogramDockWidget()
{

}

void PressureHistogramDockWidget::updateHistogram()
{
        const auto levels = m_config->pressureLevels();
        const int VALUES_MULTIPLIER {100'000};

        int data[levels];
        std::memset(data, 0, sizeof data);

        auto pressure = m_inspector->pressure();

        std::set<uint> unique_values;

        for (int i = 0; i < pressure.size(); ++i) {
            qreal p = pressure[i];
            uint index = p * levels;
            data[index] += 1;
            if (p > 0) {
                unique_values.insert(p * VALUES_MULTIPLIER);
            }
        }

        int histogram_max = 0;
        auto series = new QLineSeries();

        for (int i = 0; i < levels; ++i) {
            series->append(QPoint(i, data[i]));
            histogram_max = std::max(histogram_max, data[i]);
        }

        std::vector<uint> values(unique_values.begin(), unique_values.end());
        std::sort(values.begin(), values.end());
        QStringList values_string_list;
        values_string_list.reserve(values.size());
        for (int i = 0; i < values.size(); ++i) {
            values_string_list << QString::number(qreal(values[i]) / VALUES_MULTIPLIER);
        }
        m_valuesModel->setStringList(values_string_list);

        series->setName(tr("Pressure (%1 values)").arg(unique_values.size()));

        m_chart->removeAllSeries();
        m_chart->addSeries(series);

        m_xAxis->setRange(0, levels);
        series->attachAxis(m_xAxis);

        m_yAxis->setRange(0, histogram_max);
        series->attachAxis(m_yAxis);
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
