#pragma once

#include <QDockWidget>

class QQuickWidget;

class QuickDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit QuickDockWidget(const QString& objectName, const QString& title, const QString& qmlSource, QWidget *parent = nullptr);

    void configure();

    virtual void saveSettings() const;
    virtual void loadSettings();

protected:
    QMap<QString, QObject*> m_qmlProperties;
    QString m_qmlSource;
    QQuickWidget *m_quickWidget{nullptr};
};

