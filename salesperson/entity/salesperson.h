#ifndef SALESPERSON_H
#define SALESPERSON_H

#include <QObject>

namespace Entity {
class Salesperson
{
    Q_GADGET

    Q_PROPERTY(QString username READ username WRITE setUsername FINAL)
    Q_PROPERTY(QString password READ password WRITE setPassword FINAL)
    Q_PROPERTY(QString email READ email WRITE setEmail FINAL)
    Q_PROPERTY(QString forename READ forename WRITE setForename FINAL)
    Q_PROPERTY(QString surname READ surname WRITE setSurname FINAL)
    Q_PROPERTY(int creditLimit READ creditLimit WRITE setCreditLimit FINAL)

public:
    static Salesperson fromMap(const QVariantMap &data);
    static const int userTypeId;

    Salesperson();

    QString username() const;
    void setUsername(const QString &newUsername);

    QString password() const;
    void setPassword(const QString &newPassword);

    QString email() const;
    void setEmail(const QString &newEmail);

    QString forename() const;
    void setForename(const QString &newForename);

    QString surname() const;
    void setSurname(const QString &newSurname);

    int creditLimit() const;
    void setCreditLimit(int newCreditLimit);

    bool isPosteable() const;

    QByteArray toJson() const;

private:
    QJsonObject prepareUser() const;
    QJsonObject preparePersonInfo() const;

private:
    QString username_;
    QString password_;
    QString email_;

    QString forename_;
    QString surname_;

    int creditLimit_;
};
}

#endif // SALESPERSON_H
