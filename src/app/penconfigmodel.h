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

    void configure();

    int size() const { return m_configs.size(); }

    QHash<int, QByteArray> roleNames() const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    Q_INVOKABLE void addConfig();
    Q_INVOKABLE void removeConfig(int index);
    Q_INVOKABLE PenConfig* config(int index) const;

signals:
    void sizeChanged();

private:
    QList<PenConfig*> m_configs;
};

