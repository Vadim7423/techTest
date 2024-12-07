#include "counts_model.h"
#include <QDateTime>

CountsModel::CountsModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_db.createCountsTable();
    m_db.loadLastCountsData(m_data);
    m_prevTime = QDateTime::currentMSecsSinceEpoch();
}

QVariant CountsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

int CountsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_data.size();
}

QVariant CountsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == NameRole) {
        return m_data[index.row()];
    }

    return QVariant();
}

bool CountsModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if(row < 0 || row > m_data.size())
        return false;

    std::lock_guard<std::mutex> lock(m_mutex);
    beginInsertRows(parent, row, row + count - 1);
    m_data.emplace_back(0);
    endInsertRows();

    m_rate = 0;
    m_prevTime = QDateTime::currentMSecsSinceEpoch();
    return true;
}

bool CountsModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if(row < 0 || row + count > m_data.size())
        return false;

    std::lock_guard<std::mutex> lock(m_mutex);
    beginRemoveRows(parent, row, row + count - 1);
    m_data.erase(m_data.begin() + row);
    endRemoveRows();

    m_rate = 0;
    m_prevTime = QDateTime::currentMSecsSinceEpoch();
    return true;
}

void CountsModel::updateData()
{
    for(auto& item : m_data) {
        ++item;
    }

    if(!m_data.empty() && QDateTime::currentMSecsSinceEpoch() - m_prevTime < 1000) {
        ++m_rate;
    }
}

void CountsModel::reset()
{
    emit dataChanged(index(0,0), index(m_data.size() - 1, 0));
}

std::mutex &CountsModel::mutex()
{
    return m_mutex;
}

int CountsModel::rate() const
{
    return m_rate;
}

void CountsModel::saveData()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_db.insertCountsData(m_data);
}

QHash<int, QByteArray> CountsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    return roles;
}
