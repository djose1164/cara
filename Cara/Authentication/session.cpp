#include "session.h"

#include <QJsonObject>

using namespace Qt::StringLiterals;

namespace Authentication
{

Session::Session()
    : user_{}
    , accessToken_{}
    , refreshToken_{}
{
}

Entity::UserEntity Session::user() const
{
    return user_;
}

void Session::setUser(const Entity::UserEntity &newUser)
{
    user_ = newUser;
}

QString Session::accessToken() const
{
    return accessToken_;
}

void Session::setAccessToken(const QString &newAccessToken)
{
    accessToken_ = newAccessToken;
}

QString Session::refreshToken() const
{
    return refreshToken_;
}

void Session::setRefreshToken(const QString &newRefreshToken)
{
    refreshToken_ = newRefreshToken;
}

Session Session::fromJson(const QJsonObject &jsonObject)
{
    Session s;
    s.setUser(Entity::UserEntity::fromJson(jsonObject["user"_L1].toObject()));
    s.setAccessToken(jsonObject["access_token"_L1].toString());
    s.setRefreshToken(jsonObject["refresh_token"].toString());
    return s;
}

} // namespace User

QDebug operator<<(QDebug dbg, const Authentication::Session &session)
{
    QDebugStateSaver saver(dbg);
    const QString refreshSliced = session.refreshToken().isEmpty()
                                      ? ""_L1
                                      : session.refreshToken().sliced(5, 20) + "..."_L1;
    const QString accessSliced = session.accessToken().isEmpty()
                                     ? ""_L1
                                     : session.accessToken().sliced(5, 20) + "..."_L1;
    dbg.noquote() << "Session(accessToken=%1, refreshToken=%2, user="_L1
                         .arg(accessSliced)
                         .arg(refreshSliced)
                  << session.user() << ")"_L1;
    return dbg;
}
