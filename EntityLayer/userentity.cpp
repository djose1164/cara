#include "userentity.h"

EntityLayer::UserEntity::UserEntity(QObject *parent)
    : QObject{parent}
{
}

bool EntityLayer::UserEntity::operator==(const UserEntity &other)
{
    return username_ == other.username_;
}

bool EntityLayer::UserEntity::operator!=(const UserEntity &other)
{
    return !(*this == other);
}

void EntityLayer::UserEntity::setUsername(const QString &username)
{
    if (username_ != username)
    {
        username_ = username;
        emit usernameChanged();
    }
}

QString EntityLayer::UserEntity::username() const
{
    return username_;
}

void EntityLayer::UserEntity::setPassword(const QString &password)
{
    if (password_ != password)
    {
        password_ = password;
        emit passwordChanged();
    }
}

QString EntityLayer::UserEntity::password() const
{
    return password_;
}
