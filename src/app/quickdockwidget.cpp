#include "quickdockwidget.h"

#include <QQmlContext>
#include <QQuickWidget>

QuickDockWidget::QuickDockWidget(const QString& objectName, const QString& title, const QString& qmlSource, QWidget *parent)
    : DockWidget(objectName, title, parent)
    , m_qmlSource(qmlSource)
    , m_quickWidget(new QQuickWidget(this))
{
    m_quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    setWidget(m_quickWidget);
}

void QuickDockWidget::configure()
{
    DockWidget::configure();

    auto context = m_quickWidget->rootContext();
    auto prop_iter = m_qmlProperties.constBegin();
    while (prop_iter != m_qmlProperties.constEnd()) {
        context->setContextProperty(prop_iter.key(), prop_iter.value());
        ++prop_iter;
    }
    m_quickWidget->setSource(QUrl(m_qmlSource));
}
