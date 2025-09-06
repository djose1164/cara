#include "userentity.h"
#include <QJsonDocument>
#include <QJsonObject>

using namespace Qt::StringLiterals;

namespace Entity
{

// TODO: Caller should pass in user json as root.
UserEntity UserEntity::fromJson(const QByteArray &data)
{
    if (data.isEmpty())
        return {};

    const auto dataJson{QJsonDocument::fromJson(data)};
    const auto dataObj{dataJson.object()};
    const auto userObj{dataObj["user"].toObject()};
    auto &&personObj{userObj[QStringLiteral("person")].toObject()};
    const auto contactDoc{personObj["contact"].toObject()};
    const auto customerObj{userObj["customer"].toObject()};

    UserEntity user{};
    user.setForename(personObj["forename"].toString());
    user.setSurname(personObj["surname"].toString());
    user.setEmail(contactDoc["email"].toString());
    user.setCustomerId(userObj["customer_id"].toInt());
    user.setUserType(static_cast<UserTypes>(userObj["user_type_id"].toInt()));
    Q_ASSERT(int(user.userType_));
    user.setUserId(userObj["id"].toInt());
    Q_ASSERT(user.userId());
    user.setUsername(userObj["username"].toString());
    user.setPhoneNumber(contactDoc["telephone"].toString());

    Q_ASSERT(user.username().isEmpty() == false);
    user.setCustomerId(customerObj["id"].toInt());

    user.setSalespersonId(userObj["seller_id"_L1].toInt());

    return user;
}

UserEntity::UserEntity()
    : username_{}
    , password_{}
    , email_{}
    , phoneNumber_{}
    , forename_{}
    , surname_{}
    , customerId_{}
    , userId_{}
    , sellerId_{}
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
    return sellerId_;
}

void UserEntity::setSalespersonId(int newSalespersonId)
{
    if (sellerId_ == newSalespersonId)
        return;
    sellerId_ = newSalespersonId;
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

QString UserEntity::phoneNumber() const
{
    return phoneNumber_;
}

void UserEntity::setPhoneNumber(const QString &newPhoneNumber)
{
    if (phoneNumber_ == newPhoneNumber)
        return;
    phoneNumber_ = newPhoneNumber;
}

QByteArray UserEntity::toJson() const
{
    QJsonObject obj;
    obj["forename"] = forename_;
    obj["surname"] = surname_;
    obj["telephone"] = phoneNumber_;
    obj["email"] = email_;
    obj["birthday"] = birthday_.toString(Qt::ISODate);

    return QJsonDocument{obj}.toJson();
}

QDate UserEntity::birthday() const
{
    return birthday_;
}

void UserEntity::setBirthday(const QDate &newBirthday)
{
    if (birthday_ == newBirthday)
        return;
    birthday_ = newBirthday;
}

Address UserEntity::address() const
{
    return address_;
}

void UserEntity::setAddress(const Address &newAddress)
{
    address_ = newAddress;
}

QString UserEntity::fullname() const
{
    return forename() + " "_L1 + surname();
}

UserEntity UserEntity::fromJson(const QJsonObject &json)
{
    if (json.isEmpty())
        return {};

    UserEntity user{};
    if (auto &&personObj{json["person"_L1].toObject()};
            not personObj.isEmpty())
    {
        user.setForename(personObj["forename"_L1].toString());
        user.setSurname(personObj["surname"_L1].toString());

        if (const auto contactDoc{personObj["contact"_L1].toObject()};
                not contactDoc.isEmpty())
        {
            user.setEmail(contactDoc["email"_L1].toString());
            user.setPhoneNumber(contactDoc["telephone"_L1].toString());
        }
    }

    user.setCustomerId(json["customer_id"_L1].toInt());
    user.setUserType(static_cast<UserTypes>(json["user_type_id"_L1].toInt()));
    Q_ASSERT(int(user.userType_));
    user.setUserId(json["id"_L1].toInt());
    Q_ASSERT(user.userId());
    user.setUsername(json["username"_L1].toString());

    Q_ASSERT(user.username().isEmpty() == false);
    if (const auto customerObj{json["customer"_L1].toObject()};
            not customerObj.isEmpty())
    {
        user.setCustomerId(customerObj["id"_L1].toInt());
    }

    user.setSalespersonId(json["seller_id"_L1].toInt());

    return user;
}

}

QDebug operator<<(QDebug dbg, const Entity::UserEntity &usr)
{
    QDebugStateSaver saver(dbg);
    dbg.nospace() << QString("User(id=%1, username=%2, email=%3)")
                         .arg(QString::number(usr.userId()))
                         .arg(usr.username())
                         .arg(usr.email());
    return dbg;
}
