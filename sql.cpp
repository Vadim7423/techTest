#include "sql.h"
#include <QDir>
#include <QByteArray>

Sql::Sql()
{
    QDir databasePath;
    QString path = databasePath.currentPath()+"/db.sqlite3";
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);
    m_db.open();
}

void Sql::createCountsTable()
{
    if(m_db.isOpen()) {
        QSqlQuery query;
        query.exec("create table counts "
                   "(id integer primary key, "
                   "data BLOB )");
    }
}

void Sql::insertCountsData(const std::vector<int>& data)
{
    if(m_db.isOpen()) {
        QSqlQuery query;

        query.prepare("SELECT * FROM counts ORDER BY id DESC LIMIT 1");
        query.exec();

        int size = data.size() * sizeof(int);
        QByteArray q_buffer(size, 'c');
        memcpy(q_buffer.data(), data.data(), size);

        if(query.next()) {
            QSqlRecord rec = query.record();

            int idCol = rec.indexOf("id");
            QString id =  query.value(idCol).toString();
            query.prepare("UPDATE counts "
                          "SET data=(:data) WHERE id=" + id + "");
        } else {
            query.prepare("INSERT INTO counts (data) "
                          "VALUES (:data)");
        }

        query.bindValue(":data", q_buffer.toBase64());
        query.exec();
    }
}

void Sql::loadLastCountsData(std::vector<int>& data)
{
    QSqlQuery query;
    if(m_db.isOpen()) {
        query.prepare("SELECT * FROM counts ORDER BY id DESC LIMIT 1");
        query.exec();

        QSqlRecord rec = query.record();

        int dataCol = rec.indexOf("data");

        while (query.next()) {
            QByteArray res = QByteArray::fromBase64(query.value(dataCol).toByteArray());
            data.resize(res.size() / sizeof(int));
            memcpy(data.data(), res.data(), res.size());
        }

    }
}

