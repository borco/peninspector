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

PressureHistogramModel::PressureHistogramModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

void PressureHistogramModel::clear()
{
    beginResetModel();
    m_pressures.clear();
    endResetModel();
    emit sizeChanged();
}

void PressureHistogramModel::addPressure(qreal pressure)
{
    // find insert location
    if (qFuzzyIsNull(pressure))
        return;

    if (m_pressures.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, 0);
        m_pressures.append({pressure, 1});
        endInsertRows();
    } else if (pressure < m_pressures[0].value) {
        beginInsertRows(QModelIndex(), 0, 0);
        m_pressures.insert(0, {pressure, 1});
        endInsertRows();
    } else if (pressure > m_pressures.last().value) {
        beginInsertRows(QModelIndex(), m_pressures.size(), m_pressures.size());
        m_pressures.append({pressure, 1});
        endInsertRows();
    } else {
        for (int i = 0; i < m_pressures.size(); ++i) {
            if (qFuzzyCompare(m_pressures[i].value, pressure)) {
                m_pressures[i].count += 1;
                emit dataChanged(index(i, 0), index(i, 0));
                break;
            }
            if (i < m_pressures.size() - 1) {
                if (pressure > m_pressures[i].value && pressure < m_pressures[i+1].value) {
                    beginInsertRows(QModelIndex(), i + 1, i + 1);
                    m_pressures.insert(i + 1, {pressure, 1});
                    endInsertRows();
                }
            }
        }
    }

    emit sizeChanged();
}

QVariant PressureHistogramModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch(section) {
        case ValueColumn: return tr("Pressure");
        case CountColumn: return tr("Count");
        default:
            break;
        }
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}

int PressureHistogramModel::rowCount(const QModelIndex &parent) const
{
    return m_pressures.size();
}

int PressureHistogramModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant PressureHistogramModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch(index.column()) {
        case ValueColumn: {
            return m_pressures[index.row()].value;
        }
        case CountColumn: {
            return m_pressures[index.row()].count;
        }
        default:
            break;
        }
    }

    return QVariant();
}
