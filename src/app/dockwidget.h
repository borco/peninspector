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
    void addToolBarAction(QAction* action);
    void addToolBarLabel(const QString& text);
    QAction *addToolBarSeparator();

private:
    void setupToolBar();
    void setupToggleAction();

    QToolBar* m_toolBar{nullptr};
    QAction* m_spacerAction{nullptr};
};
