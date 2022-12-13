/*
    Copyright 2022 by Ioan Calin Borcoman <iborco@gmail.com>

    This file is part of PenInspector.

    PenInspector is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or (at your option)
    any later version.

    PenInspector is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
    more details.

    You should have received a copy of the GNU General Public License along with
    PenInspector. If not, see <https://www.gnu.org/licenses/>.
*/

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
    void setupWidgets();
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
};
