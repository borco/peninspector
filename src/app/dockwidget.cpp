#include "dockwidget.h"

DockWidget::DockWidget(const QString &objectName, const QString &title, QWidget *parent)
    : QDockWidget(parent)
{
    setObjectName(objectName);

    auto tip = QString(tr("Toggle %1 dock").arg(title));
    setWindowTitle(title);
    setToolTip(tip);
    setStatusTip(tip);

    setAllowedAreas(Qt::AllDockWidgetAreas);
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
