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

#include "penconfigdockwidget.h"
#include "ui_penconfigdockwidget.h"

#include "penconfig.h"
#include "penconfigmodel.h"

#include <QItemSelectionModel>
#include <QSettings>

namespace {
static const char* SettingsGroupKey { "PenConfig" };
static const char* SelectedPenConfigIndexKey {"selectedPenConfigIndex"};

}

PenConfigDockWidget::PenConfigDockWidget(PenConfigModel* penConfigModel, QWidget *parent)
    : DockWidget("penConfigQuickDockWidget", tr("Pen Config"), parent)
    , m_ui(new Ui::PenConfigDockWidget)
    , m_penConfigModel(penConfigModel)
{
    m_ui->setupUi(this);

    m_ui->configView->setModel(m_penConfigModel);
    m_ui->configView->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(m_penConfigModel, &PenConfigModel::modelReset, this, &PenConfigDockWidget::updateConfigInfo);

    connect(m_ui->configView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &PenConfigDockWidget::updateConfigInfo);

    connect(m_ui->name, &QLineEdit::textChanged, this, [this](const QString& value){
        auto index = selectedPenConfigIndex();
        m_penConfigModel->setData(m_penConfigModel->index(index), value, PenConfigModel::NameRole);
    });

    connect(m_ui->pressureLevels, &QLineEdit::textChanged, this, [this](const QString& value){
        auto index = selectedPenConfigIndex();
        m_penConfigModel->setData(m_penConfigModel->index(index), value, PenConfigModel::PressureLevelsRole);
    });

    connect(m_ui->tilt, &QLineEdit::textChanged, this, [this](const QString& value){
        auto index = selectedPenConfigIndex();
        m_penConfigModel->setData(m_penConfigModel->index(index), value, PenConfigModel::TiltRole);
    });

    addToolBarSeparator();

    auto action = new QAction(this);
    action->setIcon(QIcon(":/icons/config-add.svg"));
    connect(action, &QAction::triggered, this, &PenConfigDockWidget::appendConfig);
    addToolBarAction(action);

    action = new QAction(this);
    action->setIcon(QIcon(":/icons/config-remove.svg"));
    connect(action, &QAction::triggered, this, &PenConfigDockWidget::removeSelectedConfig);
    addToolBarAction(action);
}

PenConfigDockWidget::~PenConfigDockWidget()
{
    delete m_ui;
}

void PenConfigDockWidget::updateConfigInfo()
{
    auto config = selectedPenConfig();
    m_ui->name->setText(config ? config->name() : "");
    m_ui->pressureLevels->setText(config ? QString::number(config->pressureLevels()) : "");
    m_ui->tilt->setText(config ? QString::number(config->tilt()) : "");
}

void PenConfigDockWidget::appendConfig()
{
    m_penConfigModel->appendConfig();
    selectPenConfig(m_penConfigModel->size() - 1);
}

void PenConfigDockWidget::removeSelectedConfig()
{
    auto index = selectedPenConfigIndex();
    if (index >= 0) {
        m_penConfigModel->removeConfig(index);
    }
}

void PenConfigDockWidget::saveSettings() const
{
    QSettings settings;
    settings.beginGroup(SettingsGroupKey);
    settings.setValue(SelectedPenConfigIndexKey, selectedPenConfigIndex());
    settings.endGroup();
}

void PenConfigDockWidget::loadSettings()
{
    QSettings settings;
    settings.beginGroup(SettingsGroupKey);
    selectPenConfig(settings.value(SelectedPenConfigIndexKey).toInt());

    settings.endGroup();
}

int PenConfigDockWidget::selectedPenConfigIndex() const
{
    auto rows = m_ui->configView->selectionModel()->selectedRows();
    return rows.size() ? rows[0].row() : -1;
}

PenConfig *PenConfigDockWidget::selectedPenConfig() const
{
    return m_penConfigModel->at(selectedPenConfigIndex());
}

void PenConfigDockWidget::selectPenConfig(int index)
{
    m_ui->configView->selectionModel()->select(m_penConfigModel->index(index), QItemSelectionModel::ClearAndSelect);
}
