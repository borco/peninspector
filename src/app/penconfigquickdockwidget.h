#pragma once

#include "quickdockwidget.h"

class PenConfigModel;

class PenConfigQuickDockWidget : public QuickDockWidget
{
public:
    explicit PenConfigQuickDockWidget(PenConfigModel* penConfigModel, QWidget* parent = nullptr);
};

