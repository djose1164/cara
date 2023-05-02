#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QThread>

class DatabaseWorker : public QObject
{
    Q_OBJECT
public slots:
    void doQuery(const QString &username, const QString &password);

signals:
    void queryReady(const bool &result);
    void doingQuery(const bool &status);

private:
    bool isUsernameRegistered(const QString &username);
    bool isPasswordRegistered(const QString &password);
};

class Database : public QObject
{
    Q_OBJECT

public:
    explicit Database(QObject *parent = nullptr);

    static void createConnection(QSqlDatabase &db);

    static bool tryOpen(QSqlDatabase &db);

private:
    QString username;
    QString password;
};


class DatabaseController : public QObject
{
    Q_OBJECT
    QThread workerThread;

public:
    DatabaseController();

    ~DatabaseController();

    Q_INVOKABLE bool busy() const;

    Q_INVOKABLE void doValidate(const QString &username, const QString &password);

    void setValidate(const bool &validate);

    Q_INVOKABLE bool validate() const;


public slots:
    void handleQuery(const bool &result);
    void setBusy(const bool &busy);

signals:
    void operate(const QString &username, const QString &password);
    void busyChanged();

private:
    Database db;

    bool _busy;
    bool _validate;
};


#endif // DATABASE_H
