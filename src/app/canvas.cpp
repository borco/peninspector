#include "canvas.h"
#include "peninfo.h"

#include <QBrush>
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

    // add an action to clear the canvas
    auto action = new QAction(this);
    action->setText(tr("Clear Canvas"));
    action->setShortcutContext(Qt::ApplicationShortcut);
    action->setShortcut(QKeySequence::Delete);
    connect(action, &QAction::triggered, this, &Canvas::clear);
    addAction(action);
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
        paintAt(event->position());
        repaint();
    }
}

void Canvas::paintAt(QPointF point)
{
    QPainter painter(&m_image);
    painter.setBrush(QBrush(Qt::black));

    painter.translate(point);
    // tilt rotation is counter-clockwise and 0 is at the top
    // painter rotation is clockwise and 0 is at right
    painter.rotate(-90 - m_penInfo->tiltRotation());
    qreal size = m_brushSize * m_penInfo->pressure();
    // make the brush longer on the tilt direction
    qreal rx = size * (1 + 3 * m_penInfo->tiltAngle() / 60);
    qreal ry = size;
    painter.drawEllipse(QRect(-rx/2, -ry/2, rx, ry));
}

void Canvas::setBrushSize(int newBrushSize)
{
    if (m_brushSize == newBrushSize)
        return;
    m_brushSize = newBrushSize;
    emit brushSizeChanged();
}
