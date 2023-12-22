#include "usercredential.h"
#include <QVariantMap>
#include <QJsonDocument>
#include <QJsonObject>

namespace User::Entity {
UserCredential UserCredential::fromMap(const QVariantMap &data)
{
    UserCredential user{};
    user.username_ = data["username"].toString();
    user.email_ = data["email"].toString();
    user.password_ = data["password"].toString();
    return user;
}

UserCredential::UserCredential()
    : username_{}
    , email_{}
    , password_{}
{

}

QString UserCredential::username() const
{
    return username_;
}

void UserCredential::setUsername(const QString &newUsername)
{
    if (username_ == newUsername)
        return;
    username_ = newUsername;
}

QString UserCredential::email() const
{
    return email_;
}

void UserCredential::setEmail(const QString &newEmail)
{
    if (email_ == newEmail)
        return;
    email_ = newEmail;
}

QString UserCredential::password() const
{
    return password_;
}

void UserCredential::setPassword(const QString &newPassword)
{
    if (password_ == newPassword)
        return;
    password_ = newPassword;
}

bool UserCredential::isValid() const
{
    if (password_.isEmpty())
        return false;
    if (username_.isEmpty() && email_.isEmpty())
        return false;

    return true;
}

QByteArray UserCredential::toJson() const
{
    QJsonObject obj;
    obj["username"] = username_;
    obj["password"] = password_;

    return QJsonDocument(obj).toJson();
}
}
