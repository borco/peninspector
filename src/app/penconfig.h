#pragma once

#include <QObject>

class PenConfig : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int pressureLevels READ pressureLevels WRITE setPressureLevels NOTIFY pressureLevelsChanged)
    Q_PROPERTY(qreal tilt READ tilt WRITE setTilt NOTIFY tiltChanged)
    Q_PROPERTY(int resolution READ resolution WRITE setResolution NOTIFY resolutionChanged)
    Q_PROPERTY(qreal workWidth READ workWidth WRITE setWorkWidth NOTIFY workWidthChanged)
    Q_PROPERTY(qreal workHeight READ workHeight WRITE setWorkHeight NOTIFY workHeightChanged)

public:
    explicit PenConfig(QObject *parent = nullptr);

    QString name() const { return m_name; }
    void setName(const QString &newName);

    int pressureLevels() const { return m_pressureLevels; }
    void setPressureLevels(int newPressureLevels);

    qreal tilt() const { return m_tilt; }
    void setTilt(qreal newTilt);

    int resolution() const { return m_resolution; }
    void setResolution(int newResolution);

    qreal workWidth() const { return m_workWidth; }
    void setWorkWidth(qreal newWorkWidth);

    qreal workHeight() const { return m_workHeight; }
    void setWorkHeight(qreal newWorkHeight);

signals:
    void nameChanged();
    void pressureLevelsChanged();
    void tiltChanged();
    void resolutionChanged();
    void workWidthChanged();
    void workHeightChanged();

private:
    QString m_name;
    int m_pressureLevels {0};
    qreal m_tilt {0.0}; // degrees
    int m_resolution {0}; // LPI
    qreal m_workWidth {0.0}; // inch
    qreal m_workHeight {0.0}; // inch
};

