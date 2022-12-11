#pragma once

#include <QMainWindow>

class PenConfigModel;
class QuickDockWidget;

class QActionGroup;
class QToolButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void loadSettings();
    void saveSettings() const;
    void updatePenMenu();

    PenConfigModel* m_configs {nullptr};
    QList<QuickDockWidget*> m_docks;

    QActionGroup* m_penActionsGroup {nullptr};
    QToolButton* m_penToolButton {nullptr};
};
