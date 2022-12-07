#include "canvas.h"
#include "peninfo.h"

#include <QPainter>
#include <QResizeEvent>

namespace {
void clearCanvas(QImage &canvas, int width, int height)
{
    canvas = QImage(width, height, QImage::Format_RGB888);
    canvas.fill(QColor(Qt::white));
}
}

Canvas::Canvas(QWidget *parent)
    : QWidget{parent}
    , m_penInfo(new PenInfo(this))
{
    setTabletTracking(true);
    clearCanvas(m_image, width(), height());
}

Canvas::~Canvas()
{
    delete m_penInfo;
}

void Canvas::clear()
{
    clearCanvas(m_image, width(), height());
    update();
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    painter.drawPixmap(0, 0, QPixmap::fromImage(m_image));
}

void Canvas::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    QImage new_image;
    clearCanvas(new_image, width(), height());

    QPainter p(&new_image);
    p.drawImage(0, 0, m_image);

    m_image = new_image;

    update();
}

void Canvas::tabletEvent(QTabletEvent *event)
{
    m_penInfo->setFromEvent(event);

    if (m_penInfo->pressure() > 0) {
        drawPoint(event->position(), event->pressure(), event->xTilt(), event->yTilt());
        repaint();
    }
}

void Canvas::drawPoint(QPointF point, qreal pressure, qreal xTilt, qreal yTilt)
{
//    QRgb value = m_drawColor.rgb();
    m_image.setPixel(point.x(), point.y(), 0);
}

void Canvas::setBrushSize(int newBrushSize)
{
    if (m_brushSize == newBrushSize)
        return;
    m_brushSize = newBrushSize;
    emit brushSizeChanged();
}
