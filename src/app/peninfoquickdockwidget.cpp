#include "peninfoquickdockwidget.h"
#include "peninfo.h"

PenInfoQuickDockWidget::PenInfoQuickDockWidget(PenInfo *penInfo, QWidget *parent)
    : QuickDockWidget("penInfoQuickDockWidget",
                      tr("Pen Info"),
                      "qrc:/qml/PenInfo.qml",
                      parent)
{
    m_qmlProperties["penInfo"] = penInfo;
}
