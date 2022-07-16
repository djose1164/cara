#include "database.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

#define boolToString(boolean) ((boolean) ? "true" : "false")
void DatabaseWorker::doQuery(const QString &username, const QString &password)
{
    const bool result = isUsernameRegistered(username)
            && isPasswordRegistered(password);
    qDebug() << "doQuery's result: " << boolToString(result);
    emit queryReady(result);
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
{
    auto dbWorker = new DatabaseWorker;
    dbWorker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, dbWorker, &QObject::deleteLater);
    connect(this, &DatabaseController::operate, dbWorker, &DatabaseWorker::doQuery);
    connect(dbWorker, &DatabaseWorker::queryReady, this, &DatabaseController::handleResources);
    workerThread.start();
}

void DatabaseController::handleResources(const bool &result)
{
    qDebug() << "handleResources' result: " << boolToString(result);
    authResult = result;
}

void DatabaseController::doAuth(const QString &username, const QString &password)
{
    qDebug() << "from " << __func__ << " hola" << username << password;
    emit operate(username, password);
}

bool DatabaseController::getAuthResult() const
{
    return authResult;
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

bool Database::validate(const QString &username, const QString &password)
{
    qDebug() << "Validating from C++";

    DatabaseController dbController;
    dbController.doAuth(username, password);
    const auto result = dbController.getAuthResult();

    qDebug() << "Result from validate: " << (result ? "true" : "false");
    return result;
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

void Database::setBusy(bool busy)
{
    qDebug() << "Setting busy";
    if (_busy != busy)
    {
        _busy = busy;
        emit busyChanged();
    }
}

bool Database::busy() const
{
    qDebug() << "Getting busy";
    return _busy;
}
