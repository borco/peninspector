#include "penconfig.h"

PenConfig::PenConfig(QObject *parent)
    : QObject{parent}
{
}

void PenConfig::setName(const QString &newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}

void PenConfig::setPressureLevels(int newPressureLevels)
{
    if (m_pressureLevels == newPressureLevels)
        return;
    m_pressureLevels = newPressureLevels;
    emit pressureLevelsChanged();
}

void PenConfig::setTilt(qreal newTilt)
{
    if (qFuzzyCompare(m_tilt, newTilt))
        return;
    m_tilt = newTilt;
    emit tiltChanged();
}

void PenConfig::setResolution(int newResolution)
{
    if (m_resolution == newResolution)
        return;
    m_resolution = newResolution;
    emit resolutionChanged();
}

void PenConfig::setWorkWidth(qreal newWorkWidth)
{
    if (qFuzzyCompare(m_workWidth, newWorkWidth))
        return;
    m_workWidth = newWorkWidth;
    emit workWidthChanged();
}

void PenConfig::setWorkHeight(qreal newWorkHeight)
{
    if (qFuzzyCompare(m_workHeight, newWorkHeight))
        return;
    m_workHeight = newWorkHeight;
    emit workHeightChanged();
}
