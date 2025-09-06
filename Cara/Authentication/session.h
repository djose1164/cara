#ifndef SESSION_H
#define SESSION_H

#include "userentity.h"

#include <QQmlEngine>

namespace Authentication
{

class Session
{
    Q_GADGET
    Q_PROPERTY(::Entity::UserEntity user READ user WRITE setUser FINAL)
    Q_PROPERTY(QString accessToken READ accessToken WRITE setAccessToken FINAL)
    Q_PROPERTY(QString refreshToken READ refreshToken WRITE setRefreshToken FINAL)
    QML_VALUE_TYPE(session)

public:
    Session();

    ::Entity::UserEntity user() const;
    void setUser(const ::Entity::UserEntity &newUser);

    QString accessToken() const;
    void setAccessToken(const QString &newAccessToken);

    QString refreshToken() const;
    void setRefreshToken(const QString &newRefreshToken);

    static Session fromJson(const QJsonObject &jsonObject);

private:
    ::Entity::UserEntity user_;
    QString accessToken_;
    QString refreshToken_;
};

} // namespace Authentication

QDebug operator<<(QDebug dbg, const Authentication::Session &session);

#endif // SESSION_H
