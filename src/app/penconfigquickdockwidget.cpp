#include "penconfigquickdockwidget.h"

#include "penconfigmodel.h"

PenConfigQuickDockWidget::PenConfigQuickDockWidget(PenConfigModel *penConfigModel, QWidget *parent)
    : QuickDockWidget("penConfigQuickDockWidget",
                      tr("Pen Config"),
                      "qrc:/qml/PenConfig.qml",
                      parent)
{
    m_qmlProperties["configModel"] = penConfigModel;
}
