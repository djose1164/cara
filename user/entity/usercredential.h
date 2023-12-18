#ifndef USERcredential_H
#define USERcredential_H

#include <QObject>

namespace User::Entity {
class UserCredential
{
    Q_GADGET
    Q_PROPERTY(QString username READ username WRITE setUsername FINAL)
    Q_PROPERTY(QString email READ email WRITE setEmail FINAL)
    Q_PROPERTY(QString password READ password WRITE setPassword FINAL)
public:
    static UserCredential fromMap(const QVariantMap &data);

    UserCredential();

    QString username() const;
    void setUsername(const QString &newUsername);

    QString email() const;
    void setEmail(const QString &newEmail);

    QString password() const;
    void setPassword(const QString &newPassword);

    bool isValid() const;

    QByteArray toJson() const;

private:
    QString username_;
    QString email_;
    QString password_;
};
}

#endif // USERcredential_H
