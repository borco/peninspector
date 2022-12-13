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

#include "pressurehistorymodel.h"

PressureHistoryModel::PressureHistoryModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_pressures.resize(m_size);
    m_currentIndex = m_size - 1;
}

void PressureHistoryModel::clear()
{
    beginResetModel();
    for (int i = 0; i < m_size; ++i) {
        m_pressures[i] = Pressure{0.0, i};
    }
    m_currentIndex = m_size - 1;
    endResetModel();
}

void PressureHistoryModel::addPressure(qreal pressure)
{
    if (pressure <= 0)
        return;

    beginResetModel();
    for (int i = 0; i < m_size - 1; ++i) {
        m_pressures[i] = m_pressures[i+1];
    }
    m_pressures[m_size - 1] = Pressure{pressure, ++m_currentIndex};
    endResetModel();
}

QVariant PressureHistoryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch(section) {
        case ValueColumn: return tr("Pressure");
        case IndexColumn: return tr("Index");
        default:
            break;
        }
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}

int PressureHistoryModel::rowCount(const QModelIndex &parent) const
{
    return m_pressures.size();
}

int PressureHistoryModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant PressureHistoryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch(index.column()) {
        case ValueColumn: {
            return m_pressures[index.row()].value;
        }
        case IndexColumn: {
            return m_pressures[index.row()].index;
        }
        default:
            break;
        }
    }

    return QVariant();
}

void PressureHistoryModel::setSize(int newSize)
{
    if (m_size != newSize)
        return;

    int oldSize = m_size;
    m_size = newSize;

    beginResetModel();
    if (oldSize > newSize) {
        int delta = oldSize - newSize;
        for (int i = 0; i < newSize; ++i) {
            m_pressures[i] = m_pressures[i + delta];
        }
        m_pressures.resize(newSize);
    } else {
        m_pressures.resize(newSize);
        int delta = newSize - oldSize;
        for (int i = newSize - 1; i >= delta; --i) {
            m_pressures[i] = m_pressures[i - delta];
        }

        for (int i = delta - 1, index = m_pressures[delta].index; i >= 0; --i, --index) {
            m_pressures[i] = Pressure{0.0, index};
        }
    }
    endResetModel();
    emit sizeChanged();
}
