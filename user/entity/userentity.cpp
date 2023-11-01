#include "userentity.h"
#include <QVariantMap>

namespace Entity {
User::User()
    : username_{}
    , password_{}
    , forename_{}
    , surname_{}
    , email_{}
    , customerId_{}
    , userId_{}
    , warehouseId_{}
    , userType_{UserTypes::GUEST}
{
}

User User::fromMap(const QVariantMap &map)
{
    const auto personInfo = map["person_info"].toMap();
    User user{};
    user.fill(map);

    return user;
}

void User::fill(const QVariantMap &map)
{
    const auto personInfo = map["person_info"].toMap();
    const auto contact{personInfo["contact"].toMap()};

    setForename(personInfo["forename"].toString());
    setSurname(personInfo["surname"].toString());
    setCustomerId(personInfo["customer_id"].toInt());
    setEmail(contact["email"].toString());
    setUserType(static_cast<UserTypes>(map["user_type_id"].toInt()));
    setUserId(map["id"].toInt());
    setUsername(map["username"].toString());
    setCustomerId(personInfo["customer_id"].toInt());
    setWarehouseId(map["warehouse_id"].toInt());
}

bool User::operator==(const User &other)
{
    return username_ == other.username_;
}

bool User::operator!=(const User &other)
{
    return !(*this == other);
}

void User::setUsername(const QString &username)
{
    if (username_ != username)
    {
        username_ = username;
    }
}

QString User::username() const
{
    return username_;
}

void User::setPassword(const QString &password)
{
    if (password_ != password)
    {
        password_ = password;
    }
}

void User::setForename(const QString &forename)
{
    if (forename_ != forename)
    {
        forename_ = forename;
    }
}

void User::setSurname(const QString &surname)
{
    if (surname_ != surname)
    {
        surname_ = surname;
    }
}

void User::setUserType(UserTypes userType)
{
    userType_ = userType;
}

void User::setCustomerId(int customerId)
{
    if (customerId_ != customerId)
    {
        customerId_ = customerId;
    }
}

void User::setUserId(int userId)
{
    if (userId_ != userId)
    {
        userId_ = userId;
    }
}

void User::clear()
{
    username_.clear();
    password_.clear();
    forename_.clear();
    surname_.clear();
    email_.clear();

    setCustomerId(0);
    setUserId(0);
    setWarehouseId(0);

    setUserType(UserTypes::GUEST);
}

QString User::password() const
{
    return password_;
}

QString User::forename() const
{
    return forename_;
}

QString User::surname() const
{
    return surname_;
}

QString User::fullname() const
{
    return forename_ + " " + surname_;
}

QString User::email() const
{
    return email_;
}

void User::setEmail(const QString &email)
{
    if (email_ != email)
    {
        email_ = email;
    }
}

User::UserTypes User::userType() const
{
    return userType_;
}

int User::customerId() const
{
    return customerId_;
}

int User::userId() const
{
    return userId_;
}

int User::warehouseId() const
{
    return warehouseId_;
}

void User::setWarehouseId(int newWarehouseId)
{
    if (warehouseId_ == newWarehouseId)
        return;
    warehouseId_ = newWarehouseId;
}
}
