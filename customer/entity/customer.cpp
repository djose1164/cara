#include "customer.h"
#include <QJsonObject>

namespace Entity
{
Customer Customer::fromJson(const QJsonObject &customerObject)
{
    const auto contactObject{customerObject["contact"].toObject()};

    Customer c{};
    c.setCustomerId(customerObject["id"].toInt());
    c.setForename(contactObject["forename"].toString());
    c.setSurname(contactObject["surname"].toString());
    c.setPhoneNumber(contactObject["telephone"].toString());
    return c;
}

Customer::Customer()
    : forename_{}
    , surname_{}
    , phoneNumber_{}
    , customerId_{-1}
{}

QString Customer::forename() const
{
    return forename_;
}

void Customer::setForename(const QString &newForename)
{
    forename_ = newForename;
}

QString Customer::surname() const
{
    return surname_;
}

void Customer::setSurname(const QString &newSurname)
{
    surname_ = newSurname;
}

QString Customer::phoneNumber() const
{
    return phoneNumber_;
}

void Customer::setPhoneNumber(const QString &newPhoneNumber)
{
    phoneNumber_ = newPhoneNumber;
}

CustomerId Customer::customerId() const
{
    return customerId_;
}

void Customer::setCustomerId(CustomerId newCustomerId)
{
    customerId_ = newCustomerId;
}
}
