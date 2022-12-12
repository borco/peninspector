#pragma once

#include "dockwidget.h"

class QQuickWidget;

class QuickDockWidget : public DockWidget
{
    Q_OBJECT
public:
    explicit QuickDockWidget(const QString& objectName, const QString& title, const QString& qmlSource, QWidget *parent = nullptr);

    void configure() override;

protected:
    QMap<QString, QObject*> m_qmlProperties;
    QString m_qmlSource;
    QQuickWidget *m_quickWidget{nullptr};
};

