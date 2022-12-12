#include "peninspector.h"

#include "peninfo.h"

PenInspector::PenInspector(PenInfo* penInfo, QObject *parent)
    : QObject{parent}
    , m_penInfo(penInfo)
{
    connect(m_penInfo, &PenInfo::pressureChanged, this, &PenInspector::onPenInfoPressureChanged);
}

void PenInspector::clearPressure()
{
    m_pressure.clear();
    emit pressureChanged();
}

void PenInspector::onPenInfoPressureChanged()
{
    if (m_penInfo->pressure() > 0) {
        m_pressure.append(m_penInfo->pressure());
        emit pressureChanged();
    }
}
