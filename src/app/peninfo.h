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

private:

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
};
