#pragma once

#include <QDockWidget>

class QToolBar;

class DockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockWidget(const QString& objectName, const QString& title, QWidget *parent = nullptr);

    virtual void saveSettings() const;
    virtual void loadSettings();
    virtual void configure();

protected:
    void addToolbarAction(QAction* action);
    QAction *addToolbarWidget(QWidget* widget);
    QAction *addToolbarSeparator();

    QToolBar* m_toolBar{nullptr};
    QAction* m_spacerAction{nullptr};

private:
    void setupToolBar();
    void setupToggleAction();
};
