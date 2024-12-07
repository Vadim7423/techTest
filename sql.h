#ifndef SQL_H
#define SQL_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>

class Sql
{
public:
    Sql();

    void createCountsTable();
    void insertCountsData(const std::vector<int>& data);
    void loadLastCountsData(std::vector<int>& data);

private:

    QSqlDatabase m_db;

};

#endif // SQL_H
