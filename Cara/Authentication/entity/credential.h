#ifndef USERcredential_H
#define USERcredential_H

#include <QObject>
#include <QQmlEngine>

namespace Authentication::Entity {
class Credential
{
    Q_GADGET
    QML_VALUE_TYPE(credential)
    Q_PROPERTY(QString username READ username WRITE setUsername FINAL)
    Q_PROPERTY(QString email READ email WRITE setEmail FINAL)
    Q_PROPERTY(QString password READ password WRITE setPassword FINAL)
public:
    static Credential fromMap(const QVariantMap &data);

    Credential();

    bool operator==(const Credential &other) const;

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

QDebug operator<<(QDebug dbg, const Authentication::Entity::Credential &c);

#endif // USERcredential_H
