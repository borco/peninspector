#include "mainwindow.h"

#include "canvas.h"
#include "penconfig.h"
#include "penconfigmodel.h"
#include "penconfigquickdockwidget.h"
#include "peninfoquickdockwidget.h"
#include "pressurehistogram.h"

#include <QActionGroup>
#include <QInputDevice>
#include <QMenu>
#include <QMenuBar>
#include <QSettings>
#include <QTabletEvent>
#include <QToolBar>
#include <QToolButton>

namespace {
static const char* GeometryKey {"MainWindow/geometry"};
static const char* StateKey {"MainWindow/state"};
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_configs(new PenConfigModel(this))
{
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
    m_docks << new PressureHistogram(pen_info, this);

    for (const auto& dock: m_docks) {
        dock->configure();
        auto action = dock->toggleViewAction();
        windows_menu->addAction(action);
        tool_bar->addAction(action);
        addDockWidget(Qt::LeftDockWidgetArea, dock);
    }

    tool_bar->addSeparator();

    m_penActionsGroup = new QActionGroup(this);
    m_penToolButton = new QToolButton(this);
    m_penToolButton->setText("pen");
    m_penToolButton->setPopupMode(QToolButton::InstantPopup);
    tool_bar->addWidget(m_penToolButton);

    loadSettings();

    connect(m_configs, &PenConfigModel::sizeChanged, this, &MainWindow::updatePenMenu);

    updatePenMenu();
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

    restoreGeometry(settings.value(GeometryKey).toByteArray());
    restoreState(settings.value(StateKey).toByteArray());

    for (const auto& dock: m_docks) {
        dock->loadSettings();
    }

    m_configs->loadSettings();
}

void MainWindow::saveSettings() const
{
    qDebug() << "Saving main window settings...";
    QSettings settings;
    settings.setValue(GeometryKey, saveGeometry());
    settings.setValue(StateKey, saveState());

    for (const auto& dock: m_docks) {
        dock->saveSettings();
    }

    m_configs->saveSettings();
}

void MainWindow::updatePenMenu()
{
    if (m_penToolButton->menu()) {
        qDeleteAll(m_penToolButton->menu()->actions());
        delete m_penToolButton->menu();
    }

    auto menu = new QMenu(this);

    for (int i = 0; i < m_configs->size(); ++i) {
        auto config = m_configs->config(i);
        auto action = new QAction(this);
        connect(config, &PenConfig::nameChanged, action, [config, action]() { action->setText(config->name()); });
        action->setCheckable(true);
        action->setText(config->name());
        m_penActionsGroup->addAction(action);
        menu->addAction(action);
    }

    m_penToolButton->setMenu(menu);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    qDebug() << "Closing main window...";
}
