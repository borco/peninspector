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
}

void PressureHistogramModel::addPressure(qreal pressure)
{
    // find insert location
    uint value = pressure * PressureMultiplier;

    if (value <= 0)
        return;

    if (m_pressures.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, 0);
        m_pressures.append({value, 1});
        endInsertRows();
    } else if (value < m_pressures[0].value) {
        beginInsertRows(QModelIndex(), 0, 0);
        m_pressures.insert(0, {value, 1});
        endInsertRows();
    } else if (value > m_pressures.last().value) {
        beginInsertRows(QModelIndex(), m_pressures.size(), m_pressures.size());
        m_pressures.append({value, 1});
        endInsertRows();
    } else {
        for (int i = 0; i < m_pressures.size(); ++i) {
            if (m_pressures[i].value == value) {
                m_pressures[i].count += 1;
                emit dataChanged(index(i, 0), index(i, 0));
                break;
            }
            if (i < m_pressures.size() - 1) {
                if (value > m_pressures[i].value && value < m_pressures[i+1].value) {
                    beginInsertRows(QModelIndex(), i + 1, i + 1);
                    m_pressures.insert(i + 1, {value, 1});
                    endInsertRows();
                }
            }
        }
    }
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
            return m_pressures[index.row()].value / PressureMultiplier;
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
