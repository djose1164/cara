#include "userentity.h"

EntityLayer::UserEntity::UserEntity(QObject *parent)
    : QObject{parent}
    , username_{}
    , password_{}
    , forename_{}
    , surname_{}
    , email_{}
    , customerId_{}
    , userId_{}
    , userType_{UserTypes::GUEST}
{
}

EntityLayer::UserEntity *EntityLayer::UserEntity::fromMap(const QVariantMap &map)
{
    const auto personInfo = map["person_info"].toMap();
    const auto user{new UserEntity};

    user->setForename(personInfo["forename"].toString());
    user->setSurname(personInfo["surname"].toString());
    user->setCustomerId(personInfo["customer_id"].toInt());
    user->setEmail(map["email"].toString());
    user->setUserType(static_cast<UserTypes>(map["user_type_id"].toInt()));
    user->setUserId(map["id"].toInt());

    return user;
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

void EntityLayer::UserEntity::setCustomerId(int customerId)
{
    if (customerId_ != customerId)
    {
        customerId_ = customerId;
        emit customerIdChanged();
    }
}

void EntityLayer::UserEntity::setUserId(int userId)
{
    if (userId_ != userId)
    {
        userId_ = userId;
        emit userIdChanged();
    }
}

void EntityLayer::UserEntity::clear()
{
    username_.clear();
    password_.clear();
    forename_.clear();
    surname_.clear();
    email_.clear();

    customerId_ = userId_ = 0;

    userType_ = UserTypes::GUEST;
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

QString EntityLayer::UserEntity::email() const
{
    return email_;
}

void EntityLayer::UserEntity::setEmail(const QString &email)
{
    if (email_ != email)
    {
        email_ = email;
        emit emailChanged();
    }
}

EntityLayer::UserEntity::UserTypes EntityLayer::UserEntity::userType() const
{
    return userType_;
}

int EntityLayer::UserEntity::customerId() const
{
    return customerId_.value();
}

int EntityLayer::UserEntity::userId() const
{
    return userId_;
}
