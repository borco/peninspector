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

void DockWidget::addToolBarAction(QAction *action)
{
    m_toolBar->insertAction(m_spacerAction, action);
}

void DockWidget::addToolBarLabel(const QString &text)
{
    auto label = new QLabel(text, this);
    label->setStyleSheet("QLabel{font-weight: 200; font-size: 10px;}");
    m_toolBar->insertWidget(m_spacerAction, label);
}

QAction *DockWidget::addToolBarSeparator()
{
    return m_toolBar->insertSeparator(m_spacerAction);
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
