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

private:
    bool isUsernameRegistered(const QString &username);

    bool isPasswordRegistered(const QString &password);
};

class DatabaseController : public QObject
{
    Q_OBJECT
    QThread workerThread;

public:
    DatabaseController();

    ~DatabaseController();

    void doAuth(const QString &username, const QString &password);

    bool getAuthResult() const;

public slots:
    void handleResources(const bool &result);

signals:
    void operate(const QString &username, const QString &password);

private:
    bool authResult = false;
};

class Database : public QObject
{
    Q_OBJECT

public:
    explicit Database(QObject *parent = nullptr);

    static void createConnection(QSqlDatabase &db);

    static bool tryOpen(QSqlDatabase &db);

    Q_INVOKABLE bool busy() const;

public slots:
    bool validate(const QString &username, const QString &password);

signals:
    void busyChanged();

private:
    QString username;
    QString password;

    bool _busy = false;

    DatabaseController dbController;

    bool validateUsername(const QString &username);

    bool validatePassword(const QString &password);

    bool find(const QString &by, const QString &value);

    void setBusy(bool busy);
};

#endif // DATABASE_H
