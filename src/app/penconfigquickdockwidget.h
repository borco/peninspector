// Copyright 2022 by Ioan Calin Borcoman <iborco@gmail.com>

#pragma once

#include "quickdockwidget.h"

class PenConfigModel;

class PenConfigQuickDockWidget : public QuickDockWidget
{
public:
    explicit PenConfigQuickDockWidget(PenConfigModel* penConfigModel, QWidget* parent = nullptr);
};
