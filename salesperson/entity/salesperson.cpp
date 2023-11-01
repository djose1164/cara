#include "salesperson.h"
#include <QVariantMap>
#include <QJsonObject>
#include <QJsonDocument>

namespace Entity {
const int Salesperson::userTypeId{3};

Salesperson::Salesperson()
    : username_{}
    , password_{}
    , email_{}
    , forename_{}
    , surname_{}
    , creditLimit_{1'000}
{

}

QString Salesperson::username() const
{
    return username_;
}

void Salesperson::setUsername(const QString &newUsername)
{
    username_ = newUsername;
}

QString Salesperson::password() const
{
    return password_;
}

void Salesperson::setPassword(const QString &newPassword)
{
    password_ = newPassword;
}

QString Salesperson::email() const
{
    return email_;
}

void Salesperson::setEmail(const QString &newEmail)
{
    email_ = newEmail;
}

QString Salesperson::forename() const
{
    return forename_;
}

void Salesperson::setForename(const QString &newForename)
{
    forename_ = newForename;
}

QString Salesperson::surname() const
{
    return surname_;
}

void Salesperson::setSurname(const QString &newSurname)
{
    surname_ = newSurname;
}

int Salesperson::creditLimit() const
{
    return creditLimit_;
}

void Salesperson::setCreditLimit(int newCreditLimit)
{
    creditLimit_ = newCreditLimit;
}

bool Salesperson::isPosteable() const
{
    if (forename_.isEmpty() || surname_.isEmpty() || email_.isEmpty()
        || username_.isEmpty()|| password_.isEmpty())
        return false;
    return true;
}

QByteArray Salesperson::toJson() const
{
    QJsonObject obj;
    obj["user"] = prepareUser();
    obj["person_info"] = preparePersonInfo();
    return QJsonDocument(obj).toJson();
}

QJsonObject Salesperson::prepareUser() const
{
    QJsonObject user;
    user["username"] = username_;
    user["password"] = password_;
    user["user_type_id"] = Salesperson::userTypeId;
    return user;
}

QJsonObject Salesperson::preparePersonInfo() const
{
    QJsonObject personInfo;
    personInfo["forename"] = forename_;
    personInfo["surname"] = surname_;

    QJsonObject contact;
    contact["email"] = email_;
    personInfo["contact"] = contact;
    return personInfo;
}

Salesperson Salesperson::fromMap(const QVariantMap &data)
{
    Salesperson sp{};
    sp.setForename(data["forename"].toString());
    sp.setSurname(data["surname"].toString());
    sp.setUsername(data["username"].toString());
    sp.setPassword(data.value("password").toString());
    sp.setCreditLimit(data.value("creditLimit", sp.creditLimit_).toInt());
    sp.setEmail(data["email"].toString());
    return sp;
}
}
