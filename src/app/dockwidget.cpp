#include "dockwidget.h"

#include <QAction>
#include <QLabel>
#include <QToolBar>

namespace {
static const int IconSize{12};
static const int TitleIndent{4};
}

DockWidget::DockWidget(const QString &objectName, const QString &title, QWidget *parent)
    : QDockWidget(parent)
    , m_toolBar(new QToolBar(this))
{
    setObjectName(objectName);
    setWindowTitle(title);
    setAllowedAreas(Qt::AllDockWidgetAreas);

    setupToolBar();
    setupToggleAction();
}

void DockWidget::saveSettings() const
{
}

void DockWidget::loadSettings()
{
}

void DockWidget::configure()
{
}

void DockWidget::setupToolBar()
{
    auto tool_bar = m_toolBar;
    tool_bar->setIconSize(QSize(IconSize, IconSize));
    tool_bar->setMovable(false);

    auto label = new QLabel(this);
    label->setIndent(TitleIndent);
    connect(this, &DockWidget::windowTitleChanged, label, &QLabel::setText);
    label->setText(windowTitle());

    tool_bar->addWidget(label);

    auto empty = new QWidget(this);
    empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    m_spacerAction = tool_bar->addWidget(empty);

    auto action = new QAction(this);
    action->setIcon(QIcon(":/icons/dock-float.svg"));
    action->setCheckable(true);
    action->setPriority(QAction::LowPriority);
    connect(action, &QAction::triggered, this, &DockWidget::setFloating);
    tool_bar->addAction(action);

    action = new QAction(this);
    action->setIcon(QIcon(":/icons/dock-close.svg"));
    action->setPriority(QAction::LowPriority);
    connect(action, &QAction::triggered, this, &DockWidget::close);
    tool_bar->addAction(action);

    setTitleBarWidget(tool_bar);
}

void DockWidget::setupToggleAction()
{
    auto tip = QString(tr("Toggle %1 dock").arg(windowTitle()));
    auto action = toggleViewAction();
    action->setToolTip(tip);
    action->setStatusTip(tip);
}
