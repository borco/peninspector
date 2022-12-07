#pragma once

#include "quickdockwidget.h"

class PenInfo;

class PressureHistogram : public QuickDockWidget
{
public:
    explicit PressureHistogram(PenInfo* penInfo, QWidget* parent = nullptr);
};

