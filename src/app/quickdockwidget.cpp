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

#include "quickdockwidget.h"

#include <QQmlContext>
#include <QQuickWidget>

QuickDockWidget::QuickDockWidget(const QString& objectName, const QString& title, const QString& qmlSource, QWidget *parent)
    : DockWidget(objectName, title, parent)
    , m_qmlSource(qmlSource)
    , m_quickWidget(new QQuickWidget(this))
{
    m_quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    setWidget(m_quickWidget);
}

void QuickDockWidget::configure()
{
    DockWidget::configure();

    auto context = m_quickWidget->rootContext();
    auto prop_iter = m_qmlProperties.constBegin();
    while (prop_iter != m_qmlProperties.constEnd()) {
        context->setContextProperty(prop_iter.key(), prop_iter.value());
        ++prop_iter;
    }
    m_quickWidget->setSource(QUrl(m_qmlSource));
}
