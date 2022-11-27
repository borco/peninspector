#pragma once

#include <QWidget>

class Canvas : public QWidget
{
    Q_OBJECT

public:
    explicit Canvas(QWidget *parent = nullptr);

signals:
    void tabletEventReceived(QTabletEvent *event);

protected:
    void tabletEvent(QTabletEvent *event) override;
};

