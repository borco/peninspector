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

#include "pressurehistogrammodel.h"

#include "penconfig.h"

PressureHistogramModel::PressureHistogramModel(PenConfig *config, QObject *parent)
    : QAbstractTableModel(parent)
    , m_config(config)
{
    connect(m_config, &PenConfig::pressureLevelsChanged, this, &PressureHistogramModel::clear);
}

void PressureHistogramModel::clear()
{
    beginResetModel();
    m_windowLevels.clear();
    m_totalLevels.clear();
    m_histogram.clear();
    emit totalLevelsChanged();
    emit histogramSizeChanged();
    endResetModel();
}

void PressureHistogramModel::addPressure(qreal pressure)
{
    // find insert location
    if (qFuzzyIsNull(pressure))
        return;

    int level = pressure * m_config->pressureLevels();
    m_totalLevels.insert(level);
    m_windowLevels.append(level);

    QList<int> removed_levels;
    if (m_windowSize > 0) {
        while (!m_windowLevels.isEmpty() && m_windowLevels.size() > m_windowSize) {
            removed_levels << m_windowLevels.takeFirst();
        }
    }

    const qreal delta = 1.0 / m_config->pressureLevels();
    auto it = std::lower_bound(m_histogram.begin(),
                               m_histogram.end(),
                               level,
                               [](const Pressure& pressure, int level) { return pressure.level < level; });
    int distance = (it - m_histogram.begin());
    if (it == m_histogram.end() || it->level != level) {
        beginInsertRows(QModelIndex(), distance, distance);
        m_histogram.insert(distance, {level * delta, level, 1});
        endInsertRows();
    } else {
        it->count += 1;
        emit dataChanged(index(distance, 0), index(distance, 0));
    }

    for (int level: removed_levels) {
        auto it = std::find_if(m_histogram.begin(),
                               m_histogram.end(),
                               [level](const Pressure& pressure) { return pressure.level == level; });
        int distance = (it - m_histogram.begin());
        if (it != m_histogram.end()) {
            it->count -= 1;
            if (it->count <= 0) {
                beginRemoveRows(QModelIndex(), distance, distance);
                m_histogram.removeAt(distance);
                endRemoveRows();
            } else {
                emit dataChanged(index(distance, 0), index(distance, 0));
            }
        }
    }

    emit totalLevelsChanged();
    emit histogramSizeChanged();
}

QVariant PressureHistogramModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch(section) {
        case ValueColumn: return tr("Pressure");
        case LevelColumn: return tr("Level");
        case CountColumn: return tr("Count");
        default:
            break;
        }
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}

int PressureHistogramModel::rowCount(const QModelIndex &parent) const
{
    return m_histogram.size();
}

int PressureHistogramModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant PressureHistogramModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch(index.column()) {
        case ValueColumn: return m_histogram[index.row()].value;
        case LevelColumn: return m_histogram[index.row()].level;
        case CountColumn: return m_histogram[index.row()].count;
        default:
            break;
        }
    }

    return QVariant();
}

void PressureHistogramModel::setWindowSize(int newWindowSize)
{
    if (m_windowSize == newWindowSize)
        return;
    m_windowSize = newWindowSize;
    clear();
//    qDebug() << "New histogram window:" << m_windowSize;
    emit windowSizeChanged();
}
