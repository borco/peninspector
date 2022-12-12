#pragma once

#include <QObject>

class PenConfig;
class PenInfo;

class PenInspector : public QObject
{
    Q_OBJECT

public:
    explicit PenInspector(PenInfo* penInfo, QObject *parent = nullptr);

    const QList<qreal>& pressure() const { return m_pressure; }
    void clearPressure();

signals:
    void pressureChanged();

private:
    void onPenInfoPressureChanged();

    PenInfo* m_penInfo {nullptr};
    QList<qreal> m_pressure;
};

