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

#include "peninfodockwidget.h"
#include "ui_peninfodockwidget.h"

#include "peninfo.h"

PenInfoDockWidget::PenInfoDockWidget(PenConfig* penConfig, PenInfo* penInfo, QWidget *parent)
    : DockWidget("penInfoDockWidget", tr("Pen Info"), parent)
    , m_ui(new Ui::PenInfoDockWidget)
    , m_penConfig(penConfig)
    , m_penInfo(penInfo)
{
    m_ui->setupUi(this);

    m_ui->tiltPreview->setPenConfig(m_penConfig);
    m_ui->tiltPreview->setPenInfo(m_penInfo);

    connect(m_penInfo, &PenInfo::deviceIdChanged, m_ui->deviceId, [this](int value){
        return m_ui->deviceId->setText(QString::number(value));
    });

    connect(m_penInfo, &PenInfo::deviceTypeChanged, m_ui->deviceType, [this](){
        return m_ui->deviceType->setText(m_penInfo->deviceTypeName());
    });

    connect(m_penInfo, &PenInfo::pointerTypeChanged, m_ui->pointerType, [this](){
        return m_ui->pointerType->setText(m_penInfo->pointerTypeName());
    });

    connect(m_penInfo, &PenInfo::pressureChanged, m_ui->deviceId, [this](qreal value){
        return m_ui->pressure->setText(QString::number(value));
    });

    connect(m_penInfo, &PenInfo::tangentialPressureChanged, m_ui->deviceId, [this](qreal value){
        return m_ui->tangentialPressure->setText(QString::number(value));
    });

    connect(m_penInfo, &PenInfo::xTiltChanged, m_ui->deviceId, [this](qreal value){
        return m_ui->xTilt->setText(QString::number(value));
    });

    connect(m_penInfo, &PenInfo::yTiltChanged, m_ui->deviceId, [this](qreal value){
        return m_ui->yTilt->setText(QString::number(value));
    });

    connect(m_penInfo, &PenInfo::tiltAngleChanged, m_ui->deviceId, [this](qreal value){
        return m_ui->tiltAngle->setText(QString::number(value));
    });

    connect(m_penInfo, &PenInfo::tiltRotationChanged, m_ui->deviceId, [this](qreal value){
        return m_ui->tiltRotation->setText(QString::number(value));
    });

    connect(m_penInfo, &PenInfo::rotationChanged, m_ui->deviceId, [this](qreal value){
        return m_ui->rotation->setText(QString::number(value));
    });

    connect(m_penInfo, &PenInfo::zChanged, m_ui->deviceId, [this](qreal value){
        return m_ui->z->setText(QString::number(value));
    });
}

PenInfoDockWidget::~PenInfoDockWidget()
{
    delete m_ui;
}
