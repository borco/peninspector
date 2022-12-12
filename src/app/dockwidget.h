#pragma once

#include <QDockWidget>

class DockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockWidget(const QString& objectName, const QString& title, QWidget *parent = nullptr);

    virtual void saveSettings() const;
    virtual void loadSettings();
    virtual void configure();
};

