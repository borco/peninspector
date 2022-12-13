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

#include <QAbstractListModel>

class PenConfig;

class PenConfigModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int size READ size NOTIFY sizeChanged)

public:
    enum ModelRoles {
        NameRole = Qt::UserRole + 1,
        ConfigRole,
    };

    explicit PenConfigModel(QObject *parent = nullptr);
    ~PenConfigModel();

    int size() const { return m_configs.size(); }

    QHash<int, QByteArray> roleNames() const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    Q_INVOKABLE void appendConfig();
    Q_INVOKABLE void removeConfig(int index);
    Q_INVOKABLE PenConfig* at(int index) const;

    void loadSettings();
    void saveSettings();

signals:
    void sizeChanged();

private:
    QList<PenConfig*> m_configs;
};
