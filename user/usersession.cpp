#include "usersession.h"
#include "localuserstore.h"
#include <QJsonDocument>
#include <QJsonObject>

UserSession::UserSession(QObject *parent)
    : QObject{parent}
{

}

Entity::UserEntity UserSession::user() const
{
    return user_;
}

void UserSession::setUser(const Entity::UserEntity &newUser)
{
    if (user_ == newUser)
        return;
    user_ = newUser;
    emit userChanged();
}

void UserSession::buildUser(const QByteArray &data)
{
    qDebug() << "UserSession::buildUser: " << data;
    setUser(
        Entity::UserEntity::fromMap(
            QJsonDocument::fromJson(data)
                .object()
                .toVariantMap()
        )
    );

    qDebug() << "UserSession::buildUser: " << user_.userId() << " " << user_.username();
}

void UserSession::update()
{
    qDebug() << "UserSession::update: updating...";
    const Business::LocalUserStore userStore{};
    setUser(userStore.user());
    qDebug() << "UserSession::update: " << user_.userType();
}

void UserSession::clear()
{
    qDebug() << "UserSession::clear: clearing session.";
    user_.clear();
    emit userChanged();
}

Session *UserSession::session()
{
    return this;
}
