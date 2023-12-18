#include "usercredentail.h"
#include <QVariantMap>
#include <QJsonDocument>
#include <QJsonObject>

namespace User::Entity {
UserCredentail UserCredentail::fromMap(const QVariantMap &data)
{
    UserCredentail user{};
    user.username_ = data["username"].toString();
    user.email_ = data["email"].toString();
    user.password_ = data["password"].toString();
    return user;
}

UserCredentail::UserCredentail()
    : username_{}
    , email_{}
    , password_{}
{

}

QString UserCredentail::username() const
{
    return username_;
}

void UserCredentail::setUsername(const QString &newUsername)
{
    if (username_ == newUsername)
        return;
    username_ = newUsername;
}

QString UserCredentail::email() const
{
    return email_;
}

void UserCredentail::setEmail(const QString &newEmail)
{
    if (email_ == newEmail)
        return;
    email_ = newEmail;
}

QString UserCredentail::password() const
{
    return password_;
}

void UserCredentail::setPassword(const QString &newPassword)
{
    if (password_ == newPassword)
        return;
    password_ = newPassword;
}

bool UserCredentail::isValid() const
{
    if (password_.isEmpty())
        return false;
    else if (username_.isEmpty() && email_.isEmpty())
        return false;
    return true;
}

QByteArray UserCredentail::toJson() const
{
    QJsonObject obj;
    obj["username"] = username_;
    obj["password"] = password_;

    return QJsonDocument(obj).toJson();
}
}
