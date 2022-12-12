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
    PenConfig(const PenConfig& other);

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

    const PenConfig &operator =(const PenConfig& other);
    bool operator==(const PenConfig& other);

signals:
    void nameChanged();
    void pressureLevelsChanged();
    void tiltChanged();
    void resolutionChanged();
    void workWidthChanged();
    void workHeightChanged();

private:
    QString m_name {"Pen"};
    int m_pressureLevels {0};
    qreal m_tilt {0.0}; // degrees
    int m_resolution {0}; // LPI
    qreal m_workWidth {0.0}; // inch
    qreal m_workHeight {0.0}; // inch
};

QDataStream &operator<<(QDataStream &out, const PenConfig &data);
QDataStream &operator>>(QDataStream &in, PenConfig &data);

Q_DECLARE_METATYPE(PenConfig)
