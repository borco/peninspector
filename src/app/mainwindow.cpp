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
#include "penconfigmodel.h"
#include "penconfigquickdockwidget.h"
#include "peninfoquickdockwidget.h"
#include "pressurehistogramdockwidget.h"

#include <QActionGroup>
#include <QInputDevice>
#include <QMenu>
#include <QMenuBar>
#include <QSettings>
#include <QTabletEvent>
#include <QToolBar>
#include <QToolButton>

namespace {
static const char* SettingsGroupKey { "MainWindow" };
static const char* GeometryKey {"geometry"};
static const char* StateKey {"state"};
static const char* ConfigKey {"penConfig"};
static const char* ConfigIndexKey {"penConfigIndex"};
static const char* NoConfigText {"<pen not set>"};
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_configs(new PenConfigModel(this))
    , m_config(new PenConfig(this))
{
    setDockNestingEnabled(true);

    setWindowTitle(tr("PenInspector"));

    auto canvas = new Canvas(this);
    auto pen_info = canvas->penInfo();
    setCentralWidget(canvas);

    auto windows_menu = new QMenu(tr("Docks"), this);
    menuBar()->addMenu(windows_menu);

    auto tool_bar = new QToolBar(this);
    tool_bar->setObjectName("toolbar");
    tool_bar->setMovable(false);
    addToolBar(Qt::TopToolBarArea, tool_bar);

    m_docks << new PenInfoQuickDockWidget(pen_info, this);
    m_docks << new PenConfigQuickDockWidget(m_configs, this);
    m_docks << new PressureHistogramDockWidget(m_config, pen_info, this);

    for (const auto& dock: m_docks) {
        dock->configure();
        auto action = dock->toggleViewAction();
        windows_menu->addAction(action);
        tool_bar->addAction(action);
        addDockWidget(Qt::LeftDockWidgetArea, dock);
    }

    tool_bar->addSeparator();

    m_configActionsGroup = new QActionGroup(this);
    m_configToolButton = new QToolButton(this);
    m_configToolButton->setText(NoConfigText);
    m_configToolButton->setPopupMode(QToolButton::InstantPopup);
    tool_bar->addWidget(m_configToolButton);

    tool_bar->addAction(canvas->actions().at(0));

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

    m_configs->loadSettings();
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

    qDebug() << "Updated configs menu";
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

void MainWindow::setConfigIndex(int newConfigIndex)
{
    if (m_configIndex == newConfigIndex)
        return;
    m_configIndex = newConfigIndex;
    emit configIndexChanged();
}
