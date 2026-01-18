#include "supplier.h"
#include <QJsonObject>

using namespace Qt::StringLiterals;

constexpr auto idField{"id"_L1};
constexpr auto nameField{"name"_L1};

namespace ProductInventory
{

Supplier::Supplier()
    : supplierId_{}
    , name_{}
    , uniqueCode_{}
{
}

QString Supplier::name() const
{
    return name_;
}

void Supplier::setName(const QString &newName)
{
    name_ = newName;
}

QUuid Supplier::uniqueCode() const
{
    return uniqueCode_;
}

void Supplier::setUniqueCode(const QUuid &newUniqueCode)
{
    uniqueCode_ = newUniqueCode;
}

Supplier::SupplierId Supplier::supplierId() const
{
    return supplierId_;
}

void Supplier::setSupplierId(SupplierId newSupplierId)
{
    supplierId_ = newSupplierId;
}

bool Supplier::isValid() const
{
    return supplierId_ > 0 or not uniqueCode_.isNull();
}

Supplier Supplier::fromJson(const QJsonObject &json)
{
    Supplier s;
    s.setSupplierId(json[idField].toInt());
    s.setName(json[nameField].toString());
    return s;
}

} // namespace ProductInventory
