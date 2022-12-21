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

#include "penconfig.h"

PressureHistoryModel::PressureHistoryModel(PenConfig *config, QObject *parent)
    : QAbstractTableModel(parent)
    , m_config(config)
{
    connect(m_config, &PenConfig::pressureLevelsChanged, this, &PressureHistoryModel::clear);
    clear();
}

void PressureHistoryModel::clear()
{
    beginResetModel();
    m_pressures.resize(m_windowSize + 1);
    for (int i = 0; i < m_pressures.size(); ++i) {
        m_pressures[i] = {0.0, 0, i};
    }
    m_currentIndex = m_pressures.size() - 1;
    endResetModel();
}

void PressureHistoryModel::addPressure(qreal pressure)
{
    if (pressure <= 0)
        return;

    beginResetModel();
    for (int i = 0; i < m_pressures.size() - 1; ++i) {
        m_pressures[i] = m_pressures[i+1];
    }
    int level = pressure * m_config->pressureLevels();
    m_pressures[m_pressures.size() - 1] = Pressure{pressure, level, ++m_currentIndex};
    endResetModel();
}

QVariant PressureHistoryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch(section) {
        case IndexColumn: return tr("Index");
        case ValueColumn: return tr("Pressure");
        case LevelColumn: return tr("Level");
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
    return 3;
}

QVariant PressureHistoryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch(index.column()) {
        case IndexColumn: return m_pressures[index.row()].index;
        case ValueColumn: return m_pressures[index.row()].value;
        case LevelColumn: return m_pressures[index.row()].level;
        default:
            break;
        }
    }

    return QVariant();
}

void PressureHistoryModel::setWindowSize(int newSize)
{
    if (m_windowSize == newSize)
        return;

    m_windowSize = newSize;
    clear();
//    qDebug() << "New pressure history window size:" << m_windowSize;
    emit windowSizeChanged();
}
