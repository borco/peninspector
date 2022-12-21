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

class PenConfig;

class PressureHistoryModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Columns: int {
        IndexColumn = 0,
        ValueColumn,
        LevelColumn,
    };

    struct Pressure {
        qreal value {0.0};
        int level {0};
        int index {0};
    };

    explicit PressureHistoryModel(PenConfig* config, QObject *parent = nullptr);

    void clear();
    void addPressure(qreal pressure);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    int windowSize() { return m_windowSize; }
    void setWindowSize(int newSize);

    int size() const { return m_pressures.size(); }
    const Pressure& first() const { return m_pressures.first(); }
    const Pressure& last() const { return m_pressures.last(); }
    const Pressure& at(int index) const { return m_pressures[index]; }

signals:
    void windowSizeChanged();

private:
    PenConfig* m_config {nullptr};
    int m_windowSize {200};
    int m_currentIndex {0};
    QVector<Pressure> m_pressures;
};

