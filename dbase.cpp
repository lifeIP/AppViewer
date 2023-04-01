#include "dbase.h"
#include <string>

DBase::DBase(const QString& DBname)
{
    DBERROR = 0;
    sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(DBname);
    if (!sdb.open()) {
        DBERROR = -1;
    }
}


int DBase::add_new_app(const QString &name, const QString &path, const QString &icon)const
{
    QSqlQuery query = sdb.exec(
                "INSERT INTO applications(name, path, icon, flag, counter) VALUES('" +
                name + QString("', '") + path + QString("', '") + icon + QString("', '") +
                "0" + QString("', '") + "0" + QString("')"));
    return 0;
}

bool DBase::delete_app(const int &id) const
{
    QSqlQuery query = sdb.exec(QString("DELETE FROM aapplications WHERE KEY = %1").arg(id));
}

QString DBase::get_app_name(const int &id) const
{
    QSqlQuery query = sdb.exec(QString("SELECT name FROM applications WHERE KEY = %1").arg(id));
    if(query.next()){
        return query.value(0).toString();
    }
    return "ERROR";
}

QString DBase::get_app_path(const int &id) const
{
    QSqlQuery query = sdb.exec(QString("SELECT path FROM applications WHERE KEY = %1").arg(id));
    if(query.next()){
        return query.value(0).toString();
    }
    return "ERROR";
}

QString DBase::get_app_icon(const int &id) const
{
    QSqlQuery query = sdb.exec(QString("SELECT icon FROM applications WHERE KEY = %1").arg(id));
    if(query.next()){
        return query.value(0).toString();
    }
    return "ERROR";
}

bool DBase::get_app_flag(const int &id) const
{
    QSqlQuery query = sdb.exec(QString("SELECT flag FROM applications WHERE KEY = %1").arg(id));
    if(query.next()){
        return query.value(0).toBool();
    }
    return 0;
}

int DBase::get_app_counter(const int &id) const
{
    QSqlQuery query = sdb.exec(QString("SELECT counter FROM applications WHERE KEY = %1").arg(id));
    if(query.next()){
        return query.value(0).toInt();
    }
    return -1;
}

bool DBase::get_indexes(std::vector<int> &indexes) const
{
    QSqlQuery query(sdb);
    query.exec("SELECT KEY FROM applications");
    int counter = 0;
    while (query.next()) {
        indexes.push_back(query.value(0).toInt());
        counter++;
    }
}




