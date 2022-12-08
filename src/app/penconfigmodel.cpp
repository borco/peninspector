#include "penconfigmodel.h"

#include "penconfig.h"

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

void PenConfigModel::addConfig()
{
    insertRows(m_configs.size(), 1);
}

void PenConfigModel::removeConfig(int index)
{
    if (0 <= index && index < m_configs.size()) {
        removeRows(index, 1);
    }
}

PenConfig *PenConfigModel::config(int index) const
{
    if (0 <= index && index < m_configs.size())
        return m_configs[index];
    return nullptr;
}

void PenConfigModel::loadSettings()
{
    beginResetModel();

    auto conf = new PenConfig(this);
    conf->setName("XPPen - Deco LW");
    conf->setPressureLevels(8192);
    conf->setTilt(60);
    conf->setResolution(5080);
    conf->setWorkWidth(10);
    conf->setWorkHeight(6);
    m_configs << conf;

    endResetModel();
}

void PenConfigModel::saveSettings()
{
}

QHash<int, QByteArray> PenConfigModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[ConfigRole] = "config";
    return roles;
}
