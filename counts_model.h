#ifndef COUNTSMODEL_H
#define COUNTSMODEL_H

#include <QAbstractListModel>

#include <vector>
#include <mutex>
#include "sql.h"

class CountsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum AlarmRoles {
        NameRole = Qt::UserRole + 1
    };

    explicit CountsModel(QObject *parent = nullptr);

    /* Begin Переопределенные методы класса QAbstractItemModel. */
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    /* End Переопределенные методы класса QAbstractItemModel. */

    /** @brief Обновление счетчиков, хранящихся в @ref m_data.  */
    void updateData();
    /** @brief Обновление представления со списком счетчиков. */
    void reset();
    /** @brief Доступ к мьютексу модели.
     * @return */
    std::mutex& mutex();
    /** @brief Частота обхода счетчиков. Значение обновляется при добавлении или при удалении счетчиков.
     * @return */
    int rate() const;
    /** @brief Сохранение данных в БД. */
    Q_INVOKABLE void saveData();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    std::vector<int> m_data;
    std::mutex m_mutex;
    quint64 m_prevTime = 0;
    int m_rate = 0;
    Sql m_db;


};

#endif // COUNTSMODEL_H
