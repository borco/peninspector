#include "tiltpreview.h"

#include "penconfig.h"
#include "peninfo.h"

#include <QPainter>

TiltPreview::TiltPreview(QWidget *parent)
    : QWidget{parent}
{
}

void TiltPreview::setPenConfig(PenConfig *newPenConfig)
{
    if (m_penConfig == newPenConfig)
        return;

    if (m_penConfig) {
        disconnect(m_penConfig);
    }

    m_penConfig = newPenConfig;
    connect(m_penConfig, &PenConfig::tiltChanged, this, qOverload<>(&TiltPreview::update));

    update();
    emit penConfigChanged();
}

void TiltPreview::setPenInfo(PenInfo *newPenInfo)
{
    if (m_penInfo == newPenInfo)
        return;

    if (m_penInfo) {
        disconnect(m_penInfo);
    }

    m_penInfo = newPenInfo;
    connect(m_penInfo, &PenInfo::tiltAngleChanged, this, qOverload<>(&TiltPreview::update));
    connect(m_penInfo, &PenInfo::tiltRotationChanged, this, qOverload<>(&TiltPreview::update));
    update();
    emit penInfoChanged();
}

void TiltPreview::paintEvent(QPaintEvent *event)
{
    if (!m_penInfo || !m_penConfig)
        return;

    int side = qMin(width(), height());

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);

    painter.drawEllipse(QRect(-99, -99, 198, 198));

    painter.setBrush(QColor(0, 0, 0));
    qreal max_tilt = m_penConfig->tilt() > 0 ? m_penConfig->tilt() : 100;
    painter.rotate(360 - 90 - m_penInfo->tiltRotation());
    painter.drawRect(QRect(0, -1, 100.0 * m_penInfo->tiltAngle() / max_tilt, 2));
}
