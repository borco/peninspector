#include "mainwindow.h"

#include "canvas.h"
#include "penconfigquickdockwidget.h"
#include "peninfoquickdockwidget.h"
#include "pressurehistogram.h"

#include <QInputDevice>
#include <QMenu>
#include <QMenuBar>
#include <QSettings>
#include <QTabletEvent>

namespace {
static const char* GeometryKey {"MainWindow/geometry"};
static const char* StateKey {"MainWindow/state"};
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto canvas = new Canvas(this);
    auto pen_info = canvas->penInfo();
    setCentralWidget(canvas);

    auto windows_menu = new QMenu(tr("Docks"), this);
    menuBar()->addMenu(windows_menu);

    m_docks << new PenInfoQuickDockWidget(pen_info, this);
    m_docks << new PenConfigQuickDockWidget(this);
    m_docks << new PressureHistogram(pen_info, this);

    for (const auto& dock: m_docks) {
        dock->configure();
        auto action = dock->toggleViewAction();
        windows_menu->addAction(action);
        addDockWidget(Qt::LeftDockWidgetArea, dock);
    }

    loadSettings();
}

MainWindow::~MainWindow()
{
    for (const auto& dock: m_docks) {
        delete dock;
    }
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
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    qDebug() << "Closing main window...";
}
