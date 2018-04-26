#include "database.h"


DataBase::DataBase(QObject *parent) : QObject(parent)
{

}

DataBase::~DataBase()
{

}

void DataBase::connectToDataBase()
{
    // accesso a database SQLite
    if(!QFile(path_and_name_db).exists()){
        this->restoreDataBase();
    } else {
        this->openDataBase();
    }
    }

bool DataBase::restoreDataBase()
{
    if(this->openDataBase()){
        return (this->createTable()) ? true : false;
    } else {
        qDebug() << "Failed to restore the database";
        return false;
    }
    return false;
}

bool DataBase::openDataBase()
{
    db = QSqlDatabase::addDatabase(driver_db);
    db.setUserName(user_db);
    db.setPassword(pass_db);
    db.setHostName(hostname_db);


    bool yes_transaction;
    if (db.driver()->hasFeature(QSqlDriver::Transactions)) {
        yes_transaction = true;
    } else {
        yes_transaction = false;
    }

    switch (TYPE_DB)
    {
        case 0:
            // accesso a database QSLite
            db.setDatabaseName(path_and_name_db);
        break;

        case 1:
            // accesso a database MSACCESS .mdb
            db.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=" + path_and_name_db);
        break;

        case 2:
            // accesso a database MSACCESS .accdb
            db.setDatabaseName("DRIVER=Provider=Microsoft.ACE.OLEDB.12.0;FIL={MS Access};DBQ=" + path_and_name_db);
        break;

        default:
            // accesso a database MSACCESS .mdb
            db.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=" + path_and_name_db);
        break;
    }
    if(db.open()){
        return true;
    } else {
        return false;
    }
}

void DataBase::closeDataBase()
{
    db.close();
}

bool DataBase::createTable()
{
    QSqlDatabase::database().transaction();
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " TABLE " ("
                            "std_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            TABLE_FNAME         " VARCHAR(50)    NOT NULL,"
                            TABLE_SNAME         " VARCHAR(50)    NOT NULL,"
                            TABLE_LUOGONASCITA       " VARCHAR(50)    NOT NULL"
                        " )"
                    )){
        qDebug() << "DataBase: error of create " << TABLE;
        qDebug() << query.lastError().text();

        // Make the query inactive before doing the rollback.
        query.finish();
        QSqlDatabase::database().rollback();
        return false;
    } else {
        QSqlDatabase::database().commit();
        return true;
    }
    return false;
}

bool DataBase::inserIntoTable(const QVariantList &data)
{
    QSqlDatabase::database().transaction();
    QSqlQuery query;
    query.prepare("INSERT INTO " TABLE " ( " TABLE_FNAME ", "
                                             TABLE_SNAME ", "
                                             TABLE_LUOGONASCITA " ) "
                  "VALUES (:FName, :SName, :LuogoNascita)");
    query.bindValue(":FName",  data[0].toString());
    query.bindValue(":SName",  data[1].toString());
    query.bindValue(":LuogoNascita", data[2].toString());

    if(!query.exec()){
        qDebug() << "error insert into " << TABLE;
        qDebug() << query.lastError().text();

        // Make the query inactive before doing the rollback.
        query.finish();
        QSqlDatabase::database().rollback();
        return false;
    } else {
        QSqlDatabase::database().commit();
        return true;
    }
    return false;
}

bool DataBase::inserIntoTable(const QString &fname, const QString &sname, const QString &luogonascita)
{
    QVariantList data;
    data.append(fname);
    data.append(sname);
    data.append(luogonascita);

    if(inserIntoTable(data))
        return true;
    else
        return false;
}

 bool DataBase::removeRecord(const int id, int Rollback)
{
    QSqlDatabase::database().transaction();
    QSqlQuery query;
    query.prepare("DELETE FROM " TABLE " WHERE std_id= :ID ;");
    query.bindValue(":ID", id);

    if(!query.exec()){
        qDebug() << "error delete row " << TABLE;
        qDebug() << query.lastError().text();

        // Make the query inactive before doing the rollback.
        query.finish();
        QSqlDatabase::database().rollback();
        return false;
    } else {
        if(Rollback == 0) {
             query.finish();
            QSqlDatabase::database().commit();
        } else {
             query.finish();
            QSqlDatabase::database().rollback();
        }
        return true;
    }
    return false;
}
