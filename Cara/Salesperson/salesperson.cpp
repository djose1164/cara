#include "salesperson.h"
#include <QVariantMap>
#include <QJsonDocument>

using namespace Qt::StringLiterals;

namespace Entity {
const int Salesperson::userTypeId{3};

Salesperson::Salesperson()
    : salespersonId_{}
    , adminId_{}
    , organizationId_{}
    , username_{}
    , password_{}
    , email_{}
    , forename_{}
    , surname_{}
    , creditLimit_{1'000}
    , creditConsumed_{}
    , creditAvailable_{}
    , salespersonType_{SalespersonTypes::Associate}
{
}

bool Salesperson::operator==(const Salesperson &other) const
{
    return salespersonId_ > 0 ? (salespersonId_ == other.salespersonId_)
                              : (username_ == other.username()
                                 && email_ == other.email());
}

bool Salesperson::operator!=(const Salesperson &other) const
{
    return !(*this == other);
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
        || username_.isEmpty()|| password_.isEmpty() || organizationId_ < 1)
        return false;
    return true;
}

QByteArray Salesperson::toJson() const
{
    QJsonObject obj;
    obj["user"] = prepareUser();
    obj["salesperson_type_id"] = int(salespersonType_);
    obj["admin_id"] = adminId_;
    obj["organization_id"] = organizationId_;

    return QJsonDocument(obj).toJson();
}

QJsonObject Salesperson::prepareUser() const
{
    QJsonObject user;
    user["username"] = username_;
    user["password"] = password_;
    user["user_type_id"] = Salesperson::userTypeId;

    user["contact"] = prepareContact();;
    return user;
}

QJsonObject Salesperson::prepareWarehouse() const
{
    QJsonObject obj;
    obj["salesperson_id"] = salespersonId_;
    obj["name"] = username_ + QString::number(salespersonId_);
    return obj;
}

QJsonObject Salesperson::prepareContact() const
{
    QJsonObject contact;
    contact["email"] = email_;
    contact["forename"] = forename_;
    contact["surname"] = surname_;

    return contact;
}

Salesperson::OrganizationId Salesperson::organizationId() const
{
    return organizationId_;
}

void Salesperson::setOrganizationId(OrganizationId newOrganizationId)
{
    if (organizationId_ == newOrganizationId)
        return;
    organizationId_ = newOrganizationId;
}

QString Salesperson::fullname() const
{
    return "%1 %2"_L1.arg(forename_, surname_);
}

QString Salesperson::salespersonTypeName() const
{
    return salespersonTypeName_;
}

void Salesperson::setSalespersonTypeName(const QString &newSalespersonTypeName)
{
    if (salespersonTypeName_ == newSalespersonTypeName)
        return;
    salespersonTypeName_ = newSalespersonTypeName;
}

QString Salesperson::warehouseAddress() const
{
    return warehouseAddress_;
}

void Salesperson::setWarehouseAddress(const QString &newWarehouseAddress)
{
    if (warehouseAddress_ == newWarehouseAddress)
        return;
    warehouseAddress_ = newWarehouseAddress;
}

Salesperson::UserId Salesperson::adminId() const
{
    return adminId_;
}

void Salesperson::setAdminId(UserId newAdminId)
{
    if (adminId_ == newAdminId)
        return;
    adminId_ = newAdminId;
}

bool Salesperson::isValid() const
{
    if (salespersonId_ < 1)
        return false;
    if (isAssociate() && (creditLimit_ < 0
                          || creditAvailable_ > creditLimit_
                          || creditConsumed_ > creditLimit_))
        return false;

    return true;
}

bool Salesperson::isAssociate() const
{
    return salespersonType_ == SalespersonTypes::Associate;
}

bool Salesperson::isAdministrator() const
{
    return salespersonType_ == SalespersonTypes::Administrator;
}

int Salesperson::salespersonId() const
{
    return salespersonId_;
}

void Salesperson::setSalespersonId(SalespersonId newSalespersonId)
{
    if (salespersonId_ == newSalespersonId)
        return;
    salespersonId_ = newSalespersonId;
}

Salesperson::SalespersonTypes Salesperson::salespersonType() const
{
    return salespersonType_;
}

void Salesperson::setSalespersonType(const SalespersonTypes &newSalespersonType)
{
    if (salespersonType_ == newSalespersonType)
        return;
    salespersonType_ = newSalespersonType;
}

int Salesperson::creditAvailable() const
{
    return creditAvailable_;
}

void Salesperson::setCreditAvailable(int newCreditAvailable)
{
    if (creditAvailable_ == newCreditAvailable)
        return;
    creditAvailable_ = newCreditAvailable;
}

int Salesperson::creditConsumed() const
{
    return creditConsumed_;
}

void Salesperson::setCreditConsumed(int newCreditConsumed)
{
    if (creditConsumed_ == newCreditConsumed)
        return;
    creditConsumed_ = newCreditConsumed;
}

Salesperson Salesperson::fromMap(const QVariantMap &data)
{
    Salesperson sp{};

    sp.setForename(data.value("forename").toString());
    sp.setSurname(data.value("surname").toString());
    sp.setUsername(data.value("username").toString());
    sp.setPassword(data.value("password").toString());
    sp.setEmail(data.value("email").toString());
    sp.setAdminId(data.value("adminId").toInt());
    sp.setOrganizationId(data.value("organizationId").toInt());

    const int salespersonTypeId{data.value("salesperson_type").toJsonObject()["id"].toInt()};
    sp.setSalespersonType(static_cast<SalespersonTypes>(salespersonTypeId));

    return sp;
}

Salesperson Salesperson::fromJson(const QByteArray &data)
{
    const auto spObj{QJsonDocument::fromJson(data).object()};
    Salesperson sp{};

    sp.setSalespersonId(spObj.value("id").toInt());
    sp.setCreditLimit(spObj.value("credit_limit").toInt());
    sp.setCreditAvailable(spObj.value("credit_available").toInt());
    sp.setCreditConsumed(spObj.value("credit_consumed").toInt());
    sp.setSalespersonId(spObj.value("id").toInt());
    sp.setOrganizationId(spObj.value("organization_id").toInt());
    sp.setForename(spObj["forename"_L1].toString());
    sp.setSurname(spObj["surname"_L1].toString());

    const auto typesMap{spObj.value("salesperson_type").toObject()};
    sp.setSalespersonTypeName(typesMap["name"].toString());
    sp.setSalespersonType(static_cast<SalespersonTypes>(typesMap["id"].toInt()));

    const auto warehouse{spObj["warehouse"].toObject()};
    sp.setWarehouseAddress(warehouse["address_name"].toString());

    return sp;
}
}
