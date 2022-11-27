#include "canvas.h"

Canvas::Canvas(QWidget *parent)
    : QWidget{parent}
{
    setTabletTracking(true);
}

void Canvas::tabletEvent(QTabletEvent *event)
{
    emit tabletEventReceived(event);
}
