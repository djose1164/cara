#include "userentity.h"
#include <QVariantMap>

namespace Entity {
UserEntity::UserEntity()
    : username_{}
    , password_{}
    , forename_{}
    , surname_{}
    , email_{}
    , customerId_{}
    , userId_{}
    , warehouseId_{}
    , userType_{UserTypes::Guest}
{
}

UserEntity UserEntity::fromMap(const QVariantMap &map)
{
    const auto personInfo = map["person_info"].toMap();
    const auto contact{personInfo["contact"].toMap()};
    qDebug() << map["username"].toString();
    
    UserEntity user{};
    user.setForename(personInfo["forename"].toString());
    user.setSurname(personInfo["surname"].toString());
    user.setCustomerId(personInfo["customer_id"].toInt());
    user.setEmail(contact["email"].toString());
    user.setUserType(static_cast<UserTypes>(map["user_type_id"].toInt()));
    user.setUserId(map["id"].toInt());
    user.setUsername(map["username"].toString());
    user.setCustomerId(personInfo["customer_id"].toInt());
    user.setWarehouseId(map["warehouse_id"].toInt());

    return user;
}

bool UserEntity::operator==(const UserEntity &other)
{
    return username_ == other.username_;
}

bool UserEntity::operator!=(const UserEntity &other)
{
    return !(*this == other);
}

void UserEntity::setUsername(const QString &username)
{
    if (username_ != username)
    {
        username_ = username;
    }
}

QString UserEntity::username() const
{
    return username_;
}

void UserEntity::setPassword(const QString &password)
{
    if (password_ != password)
    {
        password_ = password;
    }
}

void UserEntity::setForename(const QString &forename)
{
    if (forename_ != forename)
    {
        forename_ = forename;
    }
}

void UserEntity::setSurname(const QString &surname)
{
    if (surname_ != surname)
    {
        surname_ = surname;
    }
}

void UserEntity::setUserType(UserTypes userType)
{
    userType_ = userType;
}

void UserEntity::setCustomerId(int customerId)
{
    if (customerId_ != customerId)
    {
        customerId_ = customerId;
    }
}

void UserEntity::setUserId(int userId)
{
    if (userId_ != userId)
    {
        userId_ = userId;
    }
}

void UserEntity::clear()
{
    username_.clear();
    password_.clear();
    forename_.clear();
    surname_.clear();
    email_.clear();

    setCustomerId(0);
    setUserId(0);
    setWarehouseId(0);

    setUserType(UserTypes::Guest);
}

QString UserEntity::password() const
{
    return password_;
}

QString UserEntity::forename() const
{
    return forename_;
}

QString UserEntity::surname() const
{
    return surname_;
}

QString UserEntity::fullname() const
{
    return forename_ + " " + surname_;
}

QString UserEntity::email() const
{
    return email_;
}

void UserEntity::setEmail(const QString &email)
{
    if (email_ != email)
    {
        email_ = email;
    }
}

UserEntity::UserTypes UserEntity::userType() const
{
    return userType_;
}

int UserEntity::customerId() const
{
    return customerId_;
}

int UserEntity::userId() const
{
    return userId_;
}

int UserEntity::warehouseId() const
{
    return warehouseId_;
}

void UserEntity::setWarehouseId(int newWarehouseId)
{
    if (warehouseId_ == newWarehouseId)
        return;
    warehouseId_ = newWarehouseId;
}
}
