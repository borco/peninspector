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

class PressureHistogramModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Columns: int {
        ValueColumn = 0,
        LevelColumn,
        CountColumn,
    };

    struct Pressure {
        qreal value {0.0};
        int level {0};
        int count {0};
    };

    explicit PressureHistogramModel(PenConfig* config, QObject *parent = nullptr);

    void clear();
    void addPressure(qreal pressure);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    const Pressure& operator[](int index) const { return m_histogram[index]; }
    int histogramSize() const { return m_histogram.size(); }
    int totalLevels() const { return m_totalLevels.size(); }

    int windowSize() const { return m_windowSize; }
    void setWindowSize(int newWindowSize);

signals:
    void histogramSizeChanged();
    void totalLevelsChanged();
    void windowSizeChanged();

private:
    void insertPressure(int index, const Pressure& pressure);
    void removePressuresOutsideWindow();

    PenConfig* m_config = nullptr;
    QSet<int> m_totalLevels;
    QList<Pressure> m_histogram;
    QList<int> m_windowLevels;
    int m_windowSize = -1;
};
