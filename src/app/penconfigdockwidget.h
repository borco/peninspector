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

#include "dockwidget.h"

namespace Ui {
class PenConfigDockWidget;
}

class PenConfig;
class PenConfigModel;

class PenConfigDockWidget : public DockWidget
{
    Q_OBJECT

public:
    explicit PenConfigDockWidget(PenConfigModel* penConfigModel, QWidget *parent = nullptr);
    ~PenConfigDockWidget();

    void saveSettings() const override;
    void loadSettings() override;

private:
    int selectedPenConfigIndex() const;
    PenConfig* selectedPenConfig() const;
    void selectPenConfig(int index);
    void updateConfigInfo();
    void appendConfig();
    void removeSelectedConfig();

    Ui::PenConfigDockWidget *m_ui;
    PenConfigModel* m_penConfigModel = nullptr;
};

