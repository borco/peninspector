#include "penconfig.h"

namespace {
template <typename T> T getValue(QDataStream& in) {
    T value;
    in >> value;
    return value;
}
}

PenConfig::PenConfig(QObject *parent)
    : QObject{parent}
{
}

PenConfig::PenConfig(const PenConfig &other)
{
    m_name = other.m_name;
    m_pressureLevels = other.m_pressureLevels;
    m_tilt = other.m_tilt;
    m_resolution = other.m_resolution;
    m_workWidth = other.m_workWidth;
    m_workHeight = other.m_workHeight;
}

QDataStream &operator<<(QDataStream &out, const PenConfig &data)
{
    out << data.name()
        << data.pressureLevels()
        << data.tilt()
        << data.resolution()
        << data.workWidth()
        << data.workHeight()
           ;
    return out;
}

QDataStream &operator>>(QDataStream &in, PenConfig &data)
{
    data.setName(getValue<QString>(in));
    data.setPressureLevels(getValue<int>(in));
    data.setTilt(getValue<qreal>(in));
    data.setResolution(getValue<int>(in));
    data.setWorkWidth(getValue<qreal>(in));
    data.setWorkHeight(getValue<qreal>(in));

    return in;
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
