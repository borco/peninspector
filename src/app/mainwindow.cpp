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

#include "mainwindow.h"

#include "canvas.h"
#include "penconfig.h"
#include "penconfigdockwidget.h"
#include "penconfigmodel.h"
#include "peninfodockwidget.h"
#include "pressurehistogramdockwidget.h"
#include "pressurehistorydockwidget.h"

#include <QActionGroup>
#include <QInputDevice>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QSettings>
#include <QTabletEvent>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

namespace {
static const char* SettingsGroupKey { "MainWindow" };
static const char* GeometryKey {"geometry"};
static const char* StateKey {"state"};
static const char* ConfigKey {"penConfig"};
static const char* ConfigIndexKey {"penConfigIndex"};
static const char* NoConfigText {"<pen not set>"};

static const int IconSize{12};
static const int TitleIndent{4};
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_configs(new PenConfigModel(this))
    , m_config(new PenConfig(this))
{
    setupWidgets();

    loadSettings();

    connect(m_configs, &PenConfigModel::sizeChanged, this, &MainWindow::updateConfigsMenu);
    connect(m_config, &PenConfig::nameChanged, this, &MainWindow::updateConfigToolButtonText);
    connect(this, &MainWindow::configIndexChanged, this, &MainWindow::updateConfigToolButtonText);

    updateConfigsMenu();
    updateConfigToolButtonText();
}

MainWindow::~MainWindow()
{
    for (const auto& dock: m_docks) {
        delete dock;
    }

    delete m_configs;
}

void MainWindow::loadSettings()
{
    qDebug() << "Loading main window settings...";

    m_configs->loadSettings();

    QSettings settings;
    settings.beginGroup(SettingsGroupKey);
    restoreGeometry(settings.value(GeometryKey).toByteArray());
    restoreState(settings.value(StateKey).toByteArray());
    *m_config = settings.value(ConfigKey).value<PenConfig>();
    setConfigIndex(settings.value(ConfigIndexKey, -1).toInt());

    settings.endGroup();

    for (const auto& dock: m_docks) {
        dock->loadSettings();
    }
}

void MainWindow::saveSettings() const
{
    qDebug() << "Saving main window settings...";
    QSettings settings;
    settings.beginGroup(SettingsGroupKey);
    settings.setValue(GeometryKey, saveGeometry());
    settings.setValue(StateKey, saveState());
    settings.setValue(ConfigKey, QVariant::fromValue(*(m_config)));
    settings.setValue(ConfigIndexKey, m_configIndex);
    settings.endGroup();

    for (const auto& dock: m_docks) {
        dock->saveSettings();
    }

    m_configs->saveSettings();
}

void MainWindow::updateConfigsMenu()
{
    if (m_configToolButton->menu()) {
        qDeleteAll(m_configToolButton->menu()->actions());
        delete m_configToolButton->menu();
        m_configToolButton->setMenu(nullptr);
    }

    auto menu = new QMenu(this);

    for (int i = 0; i < m_configs->size(); ++i) {
        auto config = m_configs->at(i);
        auto action = new QAction(menu);
        action->setCheckable(true);
        action->setText(config->name());

        if (i == m_configIndex) {
            action->setChecked(true);
        }

        connect(config, &PenConfig::nameChanged, menu, [this, config, action, i]() {
            action->setText(config->name());
            if (m_configIndex == i) {
                m_config->setName(config->name());
            }
        });

        connect(config, &PenConfig::pressureLevelsChanged, menu, [this, config, i]() {
            if (m_configIndex == i) {
                m_config->setPressureLevels(config->pressureLevels());
            }
        });

        connect(config, &PenConfig::tiltChanged, menu, [this, config, i]() {
            if (m_configIndex == i) {
                m_config->setTilt(config->tilt());
            }
        });

        connect(action, &QAction::triggered, menu, [this, config, i]() {
            m_configToolButton->setText(config->name());
            *m_config = *config;
            setConfigIndex(i);
        });

        m_configActionsGroup->addAction(action);
        menu->addAction(action);
    }

    m_configToolButton->setMenu(menu);

//    qDebug() << "Updated configs menu";
}

void MainWindow::updateConfigToolButtonText()
{
    m_configToolButton->setText(m_configIndex >= 0 ? m_config->name() : NoConfigText);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    qDebug() << "Closing main window...";
}

void MainWindow::setupWidgets()
{
    setDockNestingEnabled(true);
    setWindowTitle(tr("PenInspector"));
    setWindowIcon(QIcon(":/icons/peninspector.svg"));

    auto widget = new QWidget(this);
    widget->setMinimumSize(200, 200);

    auto widget_layout = new QVBoxLayout(widget);
    widget_layout->setContentsMargins(0, 0, 0, 0);
    widget_layout->setSpacing(0);

    auto widget_tool_bar = new QToolBar(this);
    widget_tool_bar->setIconSize(QSize(IconSize, IconSize));
    widget_tool_bar->setMovable(false);
    widget_layout->addWidget(widget_tool_bar);

    auto label = new QLabel(tr("Canvas"), this);
    label->setIndent(TitleIndent);
    widget_tool_bar->addWidget(label);
    widget_tool_bar->addSeparator();

    auto canvas = new Canvas(this);
    widget_layout->addWidget(canvas, 1);
    widget_tool_bar->addAction(canvas->actions().at(0));

    setCentralWidget(widget);

    auto pen_info = canvas->penInfo();

    auto windows_menu = new QMenu(tr("Docks"), this);
    menuBar()->addMenu(windows_menu);

    auto app_tool_bar = new QToolBar(this);
    app_tool_bar->setObjectName("app_tool_bar");
    app_tool_bar->setMovable(false);
    addToolBar(Qt::TopToolBarArea, app_tool_bar);

    auto pen_info_dock = new PenInfoDockWidget(m_config, pen_info, this);
    addDockWidget(Qt::LeftDockWidgetArea, pen_info_dock);
    m_docks << pen_info_dock;

    auto pen_config_dock = new PenConfigDockWidget(m_configs, this);
    addDockWidget(Qt::LeftDockWidgetArea, pen_config_dock);
    m_docks << pen_config_dock;

    auto pressure_histogram_dock = new PressureHistogramDockWidget(m_config, pen_info, this);
    addDockWidget(Qt::TopDockWidgetArea, pressure_histogram_dock);
    m_docks << pressure_histogram_dock;

    auto pressure_history_dock = new PressureHistoryDockWidget(m_config, pen_info, this);
    addDockWidget(Qt::TopDockWidgetArea, pressure_history_dock);
    m_docks << pressure_history_dock;

    splitDockWidget(pen_info_dock, pen_config_dock, Qt::Horizontal);
    splitDockWidget(pressure_histogram_dock, pressure_history_dock, Qt::Vertical);

    for (const auto& dock: m_docks) {
        dock->configure();
        auto action = dock->toggleViewAction();
        windows_menu->addAction(action);
        app_tool_bar->addAction(action);
    }

    app_tool_bar->addSeparator();

    m_configActionsGroup = new QActionGroup(this);
    m_configToolButton = new QToolButton(this);
    m_configToolButton->setText(NoConfigText);
    m_configToolButton->setPopupMode(QToolButton::InstantPopup);
    app_tool_bar->addWidget(m_configToolButton);
}

void MainWindow::setConfigIndex(int newConfigIndex)
{
    if (m_configIndex == newConfigIndex)
        return;
    m_configIndex = newConfigIndex;
    emit configIndexChanged();
}
