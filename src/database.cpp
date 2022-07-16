#include "database.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

#define boolToString(boolean) ((boolean) ? "true" : "false")

void DatabaseWorker::doQuery(const QString &username, const QString &password)
{
    emit doingQuery(true);
    const bool result = isUsernameRegistered(username)
            && isPasswordRegistered(password);
    qDebug() << "doQuery's result: " << boolToString(result);
    emit queryReady(result);
    emit doingQuery(false);
}

bool DatabaseWorker::isUsernameRegistered(const QString &username)
{
    qDebug() << "isUsernameRegistered()";
    QSqlDatabase db;
    Database::createConnection(db);
    if (!Database::tryOpen(db))
        return false;

    QSqlQuery query(db);
    query.prepare(
                "SELECT username FROM users "
                "WHERE BINARY username = :username"
                );
    query.bindValue(":username", username);

    bool flag = false;
    if (query.exec() && query.next())
        flag = true;
    if (!flag)
        qDebug() << query.lastError();
    qDebug() << "flag: " << (flag ? "true" : "false");
    db.close();
    return flag;
}

bool DatabaseWorker::isPasswordRegistered(const QString &password)
{
    qDebug() << "isPasswordRegistered()";
    QSqlDatabase db;
    Database::createConnection(db);
    if (!Database::tryOpen(db))
        return false;

    QSqlQuery query(db);
    query.prepare(
                "SELECT password FROM users "
                "WHERE BINARY password = :password"
                );
    query.bindValue(":password", password);

    bool flag = false;
    if (query.exec() && query.next())
        flag = true;
    if (!flag)
        qDebug() << query.lastError();
    qDebug() << "flag: " << (flag ? "true" : "false") << "password: " << password;
    db.close();
    return flag;
}

DatabaseController::DatabaseController()
    : _busy{false}, _validate{false}
{
    auto dbWorker = new DatabaseWorker;
    dbWorker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, dbWorker, &QObject::deleteLater);
    connect(this, &DatabaseController::operate, dbWorker, &DatabaseWorker::doQuery);
    connect(dbWorker, &DatabaseWorker::doingQuery, this, &DatabaseController::setBusy);
    connect(dbWorker, &DatabaseWorker::queryReady, this, &DatabaseController::handleQuery);
    workerThread.start();
    qDebug() << "Thread executed!";
}

void DatabaseController::handleQuery(const bool &result)
{
    qDebug() << "handleQuery' result: " << boolToString(result);
    setValidate(result);
}

void DatabaseController::doValidate(const QString &username, const QString &password)
{
    qDebug() << "Validating from C++";
    emit operate(username, password);
}

void DatabaseController::setValidate(const bool &validate)
{
    _validate = validate;
}

void DatabaseController::setBusy(const bool &busy)
{
    qDebug() << "Setting busy";
    if (_busy != busy)
    {
        _busy = busy;
        emit busyChanged();
    }
}

bool DatabaseController::validate() const
{
    return _validate;
}

bool DatabaseController::busy() const
{
    qDebug() << "Getting busy";
    return _busy;
}

DatabaseController::~DatabaseController()
{
    workerThread.quit();
    workerThread.wait();
}

Database::Database(QObject *parent)
    : QObject{parent}
{
}

void Database::createConnection(QSqlDatabase &db)
{
    const QString connectionName{"Qt_MariaDB"};
    const QString databaseType{"QMYSQL"};
    if (!QSqlDatabase::contains(connectionName))
    {
        db = QSqlDatabase::addDatabase(databaseType, connectionName);
        db.setDatabaseName("test_cara");
        db.setHostName("db4free.net");
        db.setUserName("djose1164");
        db.setPassword("Kirito08.");
        db.setPort(3306);
    }
    else
        db = QSqlDatabase::database(connectionName, false);
}

bool Database::tryOpen(QSqlDatabase &db)
{
    if (!db.open())
    {
        qDebug() << "Error opening database: " << db.lastError();
        return false;
    }
    return true;
}
