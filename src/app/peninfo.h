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

#pragma once

#include <QObject>

class QTabletEvent;

class PenInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(qint64 deviceId READ deviceId WRITE setDeviceId NOTIFY deviceIdChanged)
    Q_PROPERTY(int deviceType READ deviceType WRITE setDeviceType NOTIFY deviceTypeChanged)
    Q_PROPERTY(QString deviceTypeName READ deviceTypeName NOTIFY deviceTypeChanged)
    Q_PROPERTY(int pointerType READ pointerType WRITE setPointerType NOTIFY pointerTypeChanged)
    Q_PROPERTY(QString pointerTypeName READ pointerTypeName NOTIFY pointerTypeChanged)
    Q_PROPERTY(qreal pressure READ pressure WRITE setPressure NOTIFY pressureChanged)
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY(qreal tangentialPressure READ tangentialPressure WRITE setTangentialPressure NOTIFY tangentialPressureChanged)
    Q_PROPERTY(qreal xTilt READ xTilt WRITE setXTilt NOTIFY xTiltChanged)
    Q_PROPERTY(qreal yTilt READ yTilt WRITE setYTilt NOTIFY yTiltChanged)
    Q_PROPERTY(qreal z READ z WRITE setZ NOTIFY zChanged)

    Q_PROPERTY(qreal tiltAngle READ tiltAngle WRITE setTiltAngle NOTIFY tiltAngleChanged)
    Q_PROPERTY(qreal tiltRotation READ tiltRotation WRITE setTiltRotation NOTIFY tiltRotationChanged)

public:
    explicit PenInfo(QObject *parent = nullptr);

    void setFromEvent(QTabletEvent* event);

    qint64 deviceId() const { return m_deviceId; }
    void setDeviceId(qint64 newDeviceId);

    int deviceType() const { return m_deviceType; }
    void setDeviceType(int newDeviceType);

    QString deviceTypeName() const { return m_deviceTypeName; }

    int pointerType() const { return m_pointerType; }
    void setPointerType(int newPointerType);

    QString pointerTypeName() const { return m_pointerTypeName; }

    qreal pressure() const { return m_pressure; }
    void setPressure(qreal newPressure);

    qreal rotation() const { return m_rotation; }
    void setRotation(qreal newRotation);

    qreal tangentialPressure() const { return m_tangentialPressure; }
    void setTangentialPressure(qreal newTangentialPressure);

    qreal xTilt() const { return m_xTilt; }
    void setXTilt(qreal newXTilt);

    qreal yTilt() const { return m_yTilt; }
    void setYTilt(qreal newYTilt);

    qreal z() const { return m_z; }
    void setZ(qreal newZ);

    qreal tiltAngle() const { return m_tiltAngle; }
    qreal tiltRotation() const { return m_tiltRotation; }

signals:
    void deviceIdChanged();
    void deviceTypeChanged();
    void pointerTypeChanged();
    void pressureChanged();
    void rotationChanged();
    void tangentialPressureChanged();
    void xTiltChanged();
    void yTiltChanged();
    void zChanged();

    void tiltAngleChanged();
    void tiltRotationChanged();

private:
    void updateTilt();
    void setTiltAngle(qreal newTiltAngle);
    void setTiltRotation(qreal newTiltRotation);

    qint64 m_deviceId {0L};
    int m_deviceType {0};
    int m_pointerType {0};
    QString m_pointerTypeName;
    qreal m_pressure {0.0};
    qreal m_rotation {0.0};
    qreal m_tangentialPressure {0.0};
    qreal m_xTilt {0.0};
    qreal m_yTilt {0.0};
    qreal m_z {0.0};
    QString m_deviceTypeName;

    qreal m_tiltAngle {0.0};
    qreal m_tiltRotation {0.0};
};
