#pragma once

#include <QMainWindow>

class PenConfig;
class PenConfigModel;
class PenInspector;
class DockWidget;

class QActionGroup;
class QToolButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Q_PROPERTY(int configIndex READ configIndex WRITE setConfigIndex NOTIFY configIndexChanged)

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int configIndex() const { return m_configIndex; }
    void setConfigIndex(int newConfigIndex);

signals:
    void configIndexChanged();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void loadSettings();
    void saveSettings() const;
    void updateConfigsMenu();
    void updateConfigToolButtonText();

    QList<DockWidget*> m_docks;
    PenConfigModel* m_configs {nullptr};

    int m_configIndex {-1};
    PenConfig* m_config {nullptr};
    QActionGroup* m_configActionsGroup {nullptr};
    QToolButton* m_configToolButton {nullptr};

    PenInspector* m_penInspector;
};
