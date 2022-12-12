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

