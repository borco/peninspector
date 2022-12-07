#include "pressurehistogram.h"

#include "peninfo.h"

PressureHistogram::PressureHistogram(PenInfo *penInfo, QWidget *parent)
    : QuickDockWidget("pressureHistogramQuickDockWidget",
                      tr("Pressure Histogram"),
                      "qrc:/qml/PressureHistogram.qml",
                      parent)
{
    m_qmlProperties["penInfo"] = penInfo;
}
