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

#include "penconfigquickdockwidget.h"

#include "penconfigmodel.h"

PenConfigQuickDockWidget::PenConfigQuickDockWidget(PenConfigModel *penConfigModel, QWidget *parent)
    : QuickDockWidget("penConfigQuickDockWidget",
                      tr("Pen Config"),
                      "qrc:/qml/PenConfig.qml",
                      parent)
{

    m_addConfigAction = new QAction(this);
    m_addConfigAction->setIcon(QIcon(":/icons/config-add.svg"));

    m_removeConfiAction = new QAction(this);
    m_removeConfiAction->setIcon(QIcon(":/icons/config-remove.svg"));

    m_qmlProperties["configModel"] = penConfigModel;
    m_qmlProperties["addConfigAction"] = m_addConfigAction;
    m_qmlProperties["removeConfigAction"] = m_removeConfiAction;

    addToolBarSeparator();
    addToolBarAction(m_addConfigAction);
    addToolBarAction(m_removeConfiAction);
}
