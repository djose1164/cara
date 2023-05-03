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

void EntityLayer::UserEntity::setForename(const QString &forename)
{
    if (forename_ != forename)
    {
        forename_ = forename;
        emit forenameChanged();
    }
}

void EntityLayer::UserEntity::setSurname(const QString &surname)
{
    if (surname_ != surname)
    {
        surname_ = surname;
        emit surnameChanged();
    }
}

void EntityLayer::UserEntity::setUserType(UserTypes userType)
{
    if (userType_ != userType)
    {
        userType_ = userType;
        emit userTypeChanged();
    }
}

QString EntityLayer::UserEntity::password() const
{
    return password_;
}

QString EntityLayer::UserEntity::forename() const
{
    return forename_;
}

QString EntityLayer::UserEntity::surname() const
{
    return surname_;
}

QString EntityLayer::UserEntity::fullname() const
{
    return forename_ + " " + surname_;
}

EntityLayer::UserEntity::UserTypes EntityLayer::UserEntity::userType() const
{
    return userType_;
}
