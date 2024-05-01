#include "credential.h"
#include <QVariantMap>
#include <QJsonDocument>
#include <QJsonObject>

namespace Authentication::Entity {
Credential Credential::fromMap(const QVariantMap &data)
{
    Credential user{};
    user.username_ = data["username"].toString();
    user.email_ = data["email"].toString();
    user.password_ = data["password"].toString();
    return user;
}

Credential::Credential()
    : username_{}
    , email_{}
    , password_{}
{

}

bool Credential::operator==(const Credential &other) const
{
    bool identifier{};
    if (!username_.isEmpty() && !other.username_.isEmpty())
        identifier = username_ == other.username_;
    else if (!email_.isEmpty() && !other.email_.isEmpty())
        identifier = email_ == other.email();

    return identifier && password_ == other.password_;
}

QString Credential::username() const
{
    return username_;
}

void Credential::setUsername(const QString &newUsername)
{
    if (username_ == newUsername)
        return;
    username_ = newUsername;
}

QString Credential::email() const
{
    return email_;
}

void Credential::setEmail(const QString &newEmail)
{
    if (email_ == newEmail)
        return;
    email_ = newEmail;
}

QString Credential::password() const
{
    return password_;
}

void Credential::setPassword(const QString &newPassword)
{
    if (password_ == newPassword)
        return;
    password_ = newPassword;
}

bool Credential::isValid() const
{
    if (password_.isEmpty())
        return false;
    if (username_.isEmpty() && email_.isEmpty())
        return false;

    return true;
}

QByteArray Credential::toJson() const
{
    QJsonObject obj;
    obj["username"] = username_;
    obj["password"] = password_;

    return QJsonDocument(obj).toJson();
}
}

QDebug operator<<(QDebug dbg, const Authentication::Entity::Credential &c)
{
    dbg.nospace() << "Entity::Credential(" << c.username() << ", "
                  << c.password() << ")";
    return dbg;
}
