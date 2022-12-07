#include "penconfigquickdockwidget.h"

PenConfigQuickDockWidget::PenConfigQuickDockWidget(QWidget *parent)
    : QuickDockWidget("penConfigQuickDockWidget",
                      tr("Pen Config"),
                      "qrc:/qml/PenConfig.qml",
                      parent)
{
}
