#include "customer.h"
#include <QJsonObject>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto idField{"id"_L1};
constexpr auto forenameField{"forename"_L1};
constexpr auto surnameField{"surname"_L1};
constexpr auto telephoneField{"telephone"_L1};
constexpr auto personField{"person"_L1};
}

namespace Entity
{
Customer Customer::fromJson(const QJsonObject &customerObject)
{
    const auto personObject{customerObject[personField].toObject()};

    Customer c{};
    c.setCustomerId(customerObject[idField].toInt());
    c.setForename(personObject[forenameField].toString());
    c.setSurname(personObject[surnameField].toString());
    c.setPhoneNumber(personObject[telephoneField].toString());
    return c;
}

Customer::Customer()
    : forename_{}
    , surname_{}
    , phoneNumber_{}
    , customerId_{-1}
{}

bool Customer::operator==(const Customer &rhs) const
{
    return customerId_ == rhs.customerId_;
}

bool Customer::operator!=(const Customer &rhs) const
{
    return !(*this == rhs);
}

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

bool Customer::valid() const
{
    return customerId_ > 0;
}
}
