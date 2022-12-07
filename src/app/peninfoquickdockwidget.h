#pragma once

#include "quickdockwidget.h"

class PenInfo;

class PenInfoQuickDockWidget : public QuickDockWidget
{
    Q_OBJECT
public:
    explicit PenInfoQuickDockWidget(PenInfo* penInfo, QWidget* parent = nullptr);
};

