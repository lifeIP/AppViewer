#ifndef DBASE_H
#define DBASE_H

#include "constants.h"
#include <QtSql>
#include <QString>

class DBase
{
    QSqlDatabase sdb;
public:
    int DBERROR;
    DBase(const QString& DBname);

    int     add_new_app(const QString& name, const QString& path, const QString& icon)const;
    bool    delete_app(const int& id)const;


    QString get_app_name(const int& id)const;
    QString get_app_path(const int& id)const;
    QString get_app_icon(const int& id)const;
    bool    get_app_flag(const int& id)const;
    int     get_app_counter(const int& id)const;
    bool    get_indexes(std::vector<int>& indexes)const;
};

#endif // DBASE_H
