#pragma once

#include <QMainWindow>

class PenConfig;
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
    void updateConfigsMenu();
    void updateConfig(const PenConfig *config);

    QList<QuickDockWidget*> m_docks;
    PenConfigModel* m_configs {nullptr};

    int m_configIndex {-1};
    PenConfig* m_config {nullptr};
    QActionGroup* m_configActionsGroup {nullptr};
    QToolButton* m_configToolButton {nullptr};
};
