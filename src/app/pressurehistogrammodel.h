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

#include <QAbstractTableModel>

class PressureHistogramModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Columns: int {
        ValueColumn = 0,
        CountColumn,
    };

    struct Pressure {
        uint value {0};
        uint count {0};
    };

    explicit PressureHistogramModel(QObject *parent = nullptr);

    void clear();
    void addPressure(qreal pressure);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    int size() { return m_pressures.size(); }
    const Pressure& operator[](int index) const { return m_pressures[index]; }

    inline static const qreal PressureMultiplier {100'000.0};

private:
    QList<Pressure> m_pressures;
};
