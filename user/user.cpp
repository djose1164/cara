#include "user.h"

namespace Business {
User::User(QObject *parent)
    : QObject{parent}
    , hasPreviousLogin_{} //TODO: Does Qt's engine free the memory later?
    , user_{}
{

}

QString User::accessToken() const
{
    return accessToken_;
}

void User::setAccessToken(const QString &token)
{
    accessToken_ = token;
}

QBindable<QString> User::bindableAccessToken()
{
    return &accessToken_;
}

Entity::User User::user() const
{
    return user_;
}

void User::setUser(Entity::User user)
{
    user_ = user;
}

bool User::isAdmin() const
{
    return isAdmin_;
}

QBindable<bool> User::bindableIsAdmin()
{
    return &isAdmin_;
}

bool User::isCustomer() const
{
    return isCustomer_;
}

void User::save(const QVariantMap &user)
{
    // TODO: Construct user from map & pass it to UserService::save
    // settings_.setValue("Credentials/refreshToken", "Bearer " + user["refresh_token"].toString());
}

void User::clear()
{
    // TODO: UserService::clear
}

bool User::existPreviousLogin()
{
    QSettings settings;
    const auto refreshToken{settings.value("Credentials/refreshToken").toString()};
    if (refreshToken.isEmpty())
        return false;

    return true;
}
}
