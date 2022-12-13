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

#include "penconfigmodel.h"

#include "penconfig.h"

#include <QSettings>

namespace {
namespace {
static const char* DefaultSettingsArrayKey {"ConfigModel"};
static const char* ConfigKey {"config"};
}
}

PenConfigModel::PenConfigModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

PenConfigModel::~PenConfigModel()
{
    qDeleteAll(m_configs.begin(), m_configs.end());
}

int PenConfigModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return m_configs.size();
}

QVariant PenConfigModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == NameRole) {
        return m_configs[index.row()]->name();
    } else if (role == ConfigRole) {
        return QVariant::fromValue(m_configs[index.row()]);
    }

    return QVariant();
}

bool PenConfigModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    for (int i = row; i < row + count; ++i) {
        auto config = new PenConfig(this);
        if (0 <= i && i < m_configs.size()) {
            m_configs.insert(i, config);
        } else {
            qWarning() << "Can't insert row at" << i << "so appending it!";
            m_configs.append(config);
        }
    }
    endInsertRows();
    emit sizeChanged();
    return true;
}

bool PenConfigModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for (int i=0; i < count; ++i) {
        delete m_configs.takeAt(row);
    }
    endRemoveRows();
    emit sizeChanged();
    return true;
}

void PenConfigModel::appendConfig()
{
    insertRows(m_configs.size(), 1);
}

void PenConfigModel::removeConfig(int index)
{
    if (0 <= index && index < m_configs.size()) {
        removeRows(index, 1);
    }
}

PenConfig *PenConfigModel::at(int index) const
{
    if (0 <= index && index < m_configs.size())
        return m_configs[index];
    return nullptr;
}

void PenConfigModel::loadSettings()
{
    QSettings settings;

    beginResetModel();

    qDeleteAll(m_configs);
    m_configs.clear();

    int size = settings.beginReadArray(DefaultSettingsArrayKey);
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        auto conf = new PenConfig(settings.value(ConfigKey).value<PenConfig>());
        conf->setParent(this);
        qDebug() << "Appending:" << conf->name();
        m_configs << conf;
    }
    settings.endArray();

    // add some default values if nothing loaded from the settings file
    if (m_configs.size() == 0) {
        auto conf = new PenConfig(this);
        conf->setName("XPPen - Deco LW");
        conf->setPressureLevels(8192);
        conf->setTilt(60);
        m_configs << conf;
    }

    endResetModel();

    emit sizeChanged();
}

void PenConfigModel::saveSettings()
{
    QSettings settings;
    settings.beginWriteArray(DefaultSettingsArrayKey);
    for (int i = 0; i < m_configs.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue(ConfigKey, QVariant::fromValue(*(m_configs[i])));
    }
    settings.endArray();
}

QHash<int, QByteArray> PenConfigModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[ConfigRole] = "config";
    return roles;
}
