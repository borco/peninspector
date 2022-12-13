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

public:
    explicit PenConfig(QObject *parent = nullptr);
    PenConfig(const PenConfig& other);

    QString name() const { return m_name; }
    void setName(const QString &newName);

    int pressureLevels() const { return m_pressureLevels; }
    void setPressureLevels(int newPressureLevels);

    qreal tilt() const { return m_tilt; }
    void setTilt(qreal newTilt);

    const PenConfig &operator =(const PenConfig& other);
    bool operator==(const PenConfig& other);

signals:
    void nameChanged();
    void pressureLevelsChanged();
    void tiltChanged();

private:
    QString m_name {"Pen"};
    int m_pressureLevels {0};
    qreal m_tilt {0.0}; // degrees
};

QDataStream &operator<<(QDataStream &out, const PenConfig &data);
QDataStream &operator>>(QDataStream &in, PenConfig &data);

Q_DECLARE_METATYPE(PenConfig)
