#include "userentity.h"
#include <QJsonDocument>
#include <QJsonObject>

namespace Entity {
UserEntity UserEntity::fromJson(const QByteArray &data)
{
    const auto dataJson{QJsonDocument::fromJson(data)};
    const auto dataObj{dataJson.object()};
    const auto userObj{dataObj["user"].toObject()};
    const auto contactDoc{userObj["contact"].toObject()};

    UserEntity user{};
    user.setForename(contactDoc["forename"].toString());
    user.setSurname(contactDoc["surname"].toString());
    user.setEmail(contactDoc["email"].toString());
    user.setCustomerId(userObj["customer_id"].toInt());
    user.setUserType(static_cast<UserTypes>(userObj["user_type_id"].toInt()));
    user.setUserId(userObj["id"].toInt());
    user.setUsername(userObj["username"].toString());
    qDebug() << "UserEntity::fromJson: " << user.username();
    Q_ASSERT(user.username().isEmpty() == false);
    user.setCustomerId(userObj["customer_id"].toInt());

    const auto salespersonData{QJsonDocument(userObj["salesperson"].toObject())};
    user.setSalespersonId(salespersonData["id"].toInt());

    return user;
}

UserEntity::UserEntity()
    : username_{}
    , password_{}
    , forename_{}
    , surname_{}
    , customerId_{}
    , userId_{}
    , userType_{UserTypes::Guest}
{

}

bool UserEntity::operator==(const UserEntity &other)
{
    return userId_ == other.userId_;
}

bool UserEntity::operator!=(const UserEntity &other)
{
    return !(*this == other);
}

QString UserEntity::username() const
{
    return username_;
}

void UserEntity::setUsername(const QString &newUsername)
{
    if (username_ == newUsername)
        return;
    username_ = newUsername;
}

QString UserEntity::password() const
{
    return password_;
}

void UserEntity::setPassword(const QString &newPassword)
{
    if (password_ == newPassword)
        return;
    password_ = newPassword;
}

QString UserEntity::forename() const
{
    return forename_;
}

void UserEntity::setForename(const QString &newForename)
{
    if (forename_ == newForename)
        return;
    forename_ = newForename;
}

QString UserEntity::surname() const
{
    return surname_;
}

void UserEntity::setSurname(const QString &newSurname)
{
    if (surname_ == newSurname)
        return;
    surname_ = newSurname;
}

int UserEntity::customerId() const
{
    return customerId_;
}

void UserEntity::setCustomerId(int newCustomerId)
{
    if (customerId_ == newCustomerId)
        return;
    customerId_ = newCustomerId;
}

UserEntity::UserId UserEntity::userId() const
{
    return userId_;
}

void UserEntity::setUserId(UserId newUserId)
{
    if (userId_ == newUserId)
        return;
    userId_ = newUserId;
}

UserEntity::UserTypes UserEntity::userType() const
{
    return userType_;
}

void UserEntity::setUserType(UserTypes newUserType)
{
    if (userType_ == newUserType)
        return;
    userType_ = newUserType;
}

bool UserEntity::isSalesperson() const
{
    return userType_ == UserTypes::Salesperson;
}

bool UserEntity::isAdmin() const
{
    return userType_ == UserTypes::Admin;
}

bool UserEntity::isCustomer() const
{
    return userType_ == UserTypes::Customer;
}

void UserEntity::clear()
{
    username_.clear();
    password_.clear();
    forename_.clear();
    surname_.clear();

    setCustomerId(0);
    setUserId(0);

    setUserType(UserTypes::Guest);
}

int UserEntity::salespersonId() const
{
    return salespersonId_;
}

void UserEntity::setSalespersonId(int newSalespersonId)
{
    if (salespersonId_ == newSalespersonId)
        return;
    salespersonId_ = newSalespersonId;
}

QString UserEntity::email() const
{
    return email_;
}

void UserEntity::setEmail(const QString &newEmail)
{
    if (email_ == newEmail)
        return;
    email_ = newEmail;
}
}
