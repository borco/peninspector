#pragma once

#include <QMainWindow>

class PenConfigModel;
class QuickDockWidget;

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

    PenConfigModel* m_configs {nullptr};
    QList<QuickDockWidget*> m_docks;
};
