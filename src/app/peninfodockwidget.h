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
class PenInfoDockWidget;
}

class PenConfig;
class PenInfo;

class PenInfoDockWidget : public DockWidget
{
    Q_OBJECT

public:
    explicit PenInfoDockWidget(PenConfig* penConfig, PenInfo* penInfo, QWidget *parent = nullptr);
    ~PenInfoDockWidget();

private:
    Ui::PenInfoDockWidget *m_ui {nullptr};
    PenConfig* m_penConfig {nullptr};
    PenInfo* m_penInfo {nullptr};
};

