// Copyright 2022 by Ioan Calin Borcoman <iborco@gmail.com>

#include "peninfo.h"

#include <QPointingDevice>
#include <QTabletEvent>

#include <numbers>

namespace {
const QString& deviceTypeName(int deviceType)
{
    static const QString UnknownDeviceType(QObject::tr("Unknown"));
    static const QString MouseDeviceType(QObject::tr("Mouse"));
    static const QString TouchScreenDeviceType(QObject::tr("Touch Screen"));
    static const QString TouchPadDeviceType(QObject::tr("Touch Pad"));
    static const QString PuckDeviceType(QObject::tr("Puck"));
    static const QString StylusDeviceType(QObject::tr("Stylus"));
    static const QString AirbrushDeviceType(QObject::tr("Airbrush"));
    static const QString KeyboardDeviceType(QObject::tr("Keyboard"));

    switch (QInputDevice::DeviceType(deviceType)) {
    case QInputDevice::DeviceType::Mouse:
        return MouseDeviceType;
    case QInputDevice::DeviceType::TouchScreen:
        return TouchScreenDeviceType;
    case QInputDevice::DeviceType::TouchPad:
        return TouchPadDeviceType;
    case QInputDevice::DeviceType::Puck:
        return PuckDeviceType;
    case QInputDevice::DeviceType::Stylus:
        return StylusDeviceType;
    case QInputDevice::DeviceType::Airbrush:
        return AirbrushDeviceType;
    case QInputDevice::DeviceType::Keyboard:
        return KeyboardDeviceType;
    default:
        return UnknownDeviceType;
    }
}

const QString& pointerTypeName(int pointerType)
{
    static const QString UnknownPointerType(QObject::tr("Unknown"));
    static const QString GenericPointerType(QObject::tr("Generic"));
    static const QString FingerPointerType(QObject::tr("Finger"));
    static const QString PenPointerType(QObject::tr("Pen"));
    static const QString EraserPointerType(QObject::tr("Eraser"));
    static const QString CursorPointerType(QObject::tr("Cursor"));

    switch(QPointingDevice::PointerType(pointerType)) {
    case QPointingDevice::PointerType::Generic:
        return GenericPointerType;
    case QPointingDevice::PointerType::Finger:
        return FingerPointerType;
    case QPointingDevice::PointerType::Pen:
        return PenPointerType;
    case QPointingDevice::PointerType::Eraser:
        return EraserPointerType;
    case QPointingDevice::PointerType::Cursor:
        return CursorPointerType;
    default:
        return UnknownPointerType;
    }
}
}

PenInfo::PenInfo(QObject *parent)
    : QObject{parent}
{
}

void PenInfo::setFromEvent(QTabletEvent *event)
{
    auto device = event->device();
    setDeviceId(device->systemId());
    setDeviceType(int(device->type()));

    setPointerType(int(event->pointerType()));
    setPressure(event->pressure());
    setRotation(event->rotation());
    setTangentialPressure(event->tangentialPressure());
    setXTilt(event->xTilt());
    setYTilt(event->yTilt());
    setZ(event->z());
}

void PenInfo::setDeviceId(qint64 newDeviceId)
{
    if (m_deviceId == newDeviceId)
        return;
    m_deviceId = newDeviceId;
    emit deviceIdChanged();
}

void PenInfo::setDeviceType(int newDeviceType)
{
    if (m_deviceType == newDeviceType)
        return;
    m_deviceType = newDeviceType;
    m_deviceTypeName = ::deviceTypeName(newDeviceType);
    emit deviceTypeChanged();
}

void PenInfo::setPointerType(int newPointerType)
{
    if (m_pointerType == newPointerType)
        return;
    m_pointerType = newPointerType;
    m_pointerTypeName = ::pointerTypeName(newPointerType);
    emit pointerTypeChanged();
}

void PenInfo::setPressure(qreal newPressure)
{
    if (qFuzzyCompare(m_pressure, newPressure))
        return;
    m_pressure = newPressure;
    emit pressureChanged();
}

void PenInfo::setRotation(qreal newRotation)
{
    if (qFuzzyCompare(m_rotation, newRotation))
        return;
    m_rotation = newRotation;
    emit rotationChanged();
}

void PenInfo::setTangentialPressure(qreal newTangentialPressure)
{
    if (qFuzzyCompare(m_tangentialPressure, newTangentialPressure))
        return;
    m_tangentialPressure = newTangentialPressure;
    emit tangentialPressureChanged();
}

void PenInfo::setXTilt(qreal newXTilt)
{
    if (qFuzzyCompare(m_xTilt, newXTilt))
        return;
    m_xTilt = newXTilt;
    updateTilt();
    emit xTiltChanged();
}

void PenInfo::setYTilt(qreal newYTilt)
{
    if (qFuzzyCompare(m_yTilt, newYTilt))
        return;
    m_yTilt = newYTilt;
    updateTilt();
    emit yTiltChanged();
}

void PenInfo::setZ(qreal newZ)
{
    if (qFuzzyCompare(m_z, newZ))
        return;
    m_z = newZ;
    emit zChanged();
}

void PenInfo::updateTilt()
{
    const qreal pi = std::numbers::pi_v<qreal>;
    qreal xtilt_tan = tan(m_xTilt * pi / 180);
    qreal ytilt_tan = tan(m_yTilt * pi / 180);
    qreal angle_rad = atan(sqrt(xtilt_tan * xtilt_tan + ytilt_tan * ytilt_tan));
    qreal rotation_rad = atan2(xtilt_tan, ytilt_tan);

    setTiltAngle(angle_rad * 180 / pi);
    setTiltRotation(rotation_rad * 180 / pi + 180);
}

void PenInfo::setTiltAngle(qreal newTiltAngle)
{
    if (qFuzzyCompare(m_tiltAngle, newTiltAngle))
        return;
    m_tiltAngle = newTiltAngle;
    emit tiltAngleChanged();
}

void PenInfo::setTiltRotation(qreal newTiltRotation)
{
    if (qFuzzyCompare(m_tiltRotation, newTiltRotation))
        return;
    m_tiltRotation = newTiltRotation;
    emit tiltRotationChanged();
}
