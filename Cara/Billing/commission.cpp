#include "commission.h"
#include "productentity.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimeZone>

using namespace Qt::StringLiterals;

namespace
{
    QDateTime stringToDateTime(QStringView dateString)
    {
        auto &&dt = QDateTime::fromString(dateString, Qt::ISODate);
        dt.setTimeZone(QTimeZone{Qt::UTC});
        return dt;
    }
}

namespace Billing
{

Commission::Commission()
    : rate_{}
    , amount_{}
    , commissionId_{}
    , adminId_{}
    , salespersonId_{}
    , paymentDate_{}
    , createdAt_{}
{
}

QJsonObject Commission::toJson() const
{
    QJsonArray items;
    std::ranges::for_each(commissionItems_,
                          [&items](const auto &item)
                          { items.append(item.toJson()); });
    return {
        {"commissionId"_L1, commissionId_},
        {"amount"_L1, amount_},
        {"commissionItems"_L1, items},
        {"rate"_L1, rate_},
        {"adminId"_L1, adminId_},
        {"salespersonId"_L1, salespersonId_},
        {"paymentDate"_L1, paymentDate_.isValid() ? paymentDate_.toString(Qt::ISODateWithMs) : QJsonValue{}},
        {"createdAt"_L1, createdAt_.isValid() ? createdAt_.toString(Qt::ISODate) : QJsonValue{}}
    };
}

Commission Commission::fromJson(const QJsonObject &json)
{
    Commission c;
    c.rate_ = json["rate"_L1].toString().toDouble();
    c.amount_ = json["amount"_L1].toString().toDouble();
    c.commissionId_ = json["id"_L1].toInt();
    c.adminId_ = json["admin_id"_L1].toInt();
    c.salespersonId_ = json["salesperson_id"].toInt();
    c.paymentDate_ = stringToDateTime(json["payment_date"_L1].toString());
    c.createdAt_ = stringToDateTime(json["created_at"_L1].toString());
    c.setCommissionItems(json["commission_items"_L1].toArray());
    c.setSalesperson(
        Entity::Salesperson::fromJson(
            QJsonDocument{json["salesperson"_L1].toObject()}.toJson()
        )
    );
    c.setAdmin(Entity::UserEntity::fromJson(json["admin"_L1].toObject()));
    return c;
}

Commission Commission::fromMap(const QVariantMap &map)
{
    Commission c;
    c.rate_ = map["rate"_L1].toDouble();
    //c.amount_ = map["amount"_L1].toDouble();
    const QVariantList items{map["commissionItems"_L1].toList()};
    std::ranges::for_each(items, [&](const QVariant itemVariant){
        c.commissionItems_.append(CommissionItem::fromJson(itemVariant.toJsonObject()));
    });
    c.adminId_ = map["adminId"_L1].toInt();
    c.salespersonId_ = map["salespersonId"].toInt();
    if (map.contains("paymentDate"_L1) and map["paymentDate"_L1].toDateTime().isValid())
        c.paymentDate_ = map["paymentDate"_L1].toDateTime();
    return c;
}

void Commission::validateMap(const QVariantMap &map)
{
    constexpr auto errorTemplate{"Key %1 is not present."_L1};
    if (not map.contains("salespersonId"_L1))
        throw std::invalid_argument{errorTemplate.arg("salespersonId"_L1).toUtf8()};
    else if (not map.contains("rate"_L1))
        throw std::invalid_argument{errorTemplate.arg("rate"_L1).toUtf8()};
    else if (not map.contains("adminId"_L1))
        throw std::invalid_argument{errorTemplate.arg("adminId"_L1).toUtf8()};
}

double Commission::rate() const
{
    return rate_;
}

void Commission::setRate(double newRate)
{
    rate_ = newRate;
}

double Commission::amount() const
{
    return amount_;
}

void Commission::setAmount(double newAmount)
{
    amount_ = newAmount;
}

int Commission::commissionId() const
{
    return commissionId_;
}

void Commission::setCommissionId(int newCommissionId)
{
    commissionId_ = newCommissionId;
}

int Commission::adminId() const
{
    return adminId_;
}

void Commission::setAdminId(int newAdminId)
{
    adminId_ = newAdminId;
}

int Commission::salespersonId() const
{
    return salespersonId_;
}

void Commission::setSalespersonId(int newSalespersonId)
{
    salespersonId_ = newSalespersonId;
}

QDateTime Commission::paymentDate() const
{
    return paymentDate_;
}

void Commission::setPaymentDate(const QDateTime &newPaymentDate)
{
    paymentDate_ = newPaymentDate;
}

QDateTime Commission::createdAt() const
{
    return createdAt_;
}

void Commission::setCreatedAt(const QDateTime &newCreatedAt)
{
    createdAt_ = newCreatedAt;
}

QList<CommissionItem> Commission::commissionItems() const
{
    return commissionItems_;
}

void Commission::setCommissionItems(const QList<CommissionItem> &newCommissionItems)
{
    commissionItems_ = newCommissionItems;
}

void Commission::setCommissionItems(QList<CommissionItem> &&newCommissionItems)
{
    commissionItems_ = std::move(newCommissionItems);
}

void Commission::setCommissionItems(const QJsonArray &array)
{
    for (const auto &item : array)
    {
        commissionItems_.append(CommissionItem::fromJson(item.toObject()));
    }
}

Entity::UserEntity Commission::admin() const
{
    return admin_;
}

void Commission::setAdmin(const Entity::UserEntity &newAdmin)
{
    admin_ = newAdmin;
}

bool Commission::isPatchable() const
{
    return commissionId_ > 0;
}

Entity::Salesperson Commission::salesperson() const
{
    return salesperson_;
}

void Commission::setSalesperson(const Entity::Salesperson &newSalesperson)
{
    salesperson_ = newSalesperson;
}

CommissionItem::CommissionItem()
    : CommissionItem{0, 0, 0, 0}
{
}

CommissionItem::CommissionItem(int newProductId, int newQuantity, double newUnitCommission, int newSupplierId)
    : productId_{newProductId}
    , quantity_{newQuantity}
    , supplierId_{newSupplierId}
    , unitCommission_{newUnitCommission}
    , imageUrl_{}
{
}

QJsonObject CommissionItem::toJson() const
{
    return {
        {"product_id"_L1, productId_},
        {"quantity"_L1, quantity_},
        {"supplier_id"_L1, supplierId_}
    };
}

CommissionItem CommissionItem::fromJson(const QJsonObject &json)
{
    CommissionItem ci;
    ci.productId_ = json["product_id"_L1].toInt();
    ci.quantity_ = json["quantity"_L1].toInt();
    ci.supplierId_ = json["supplier_id"_L1].toInt();
    if (json.contains("unit_commission"_L1))
        ci.unitCommission_ = json["unit_commission"_L1].toString().toDouble();
    ci.imageUrl_ = EntityLayer::Product::makeUrlToPicture(json["image_url"_L1].toString());
    return ci;
}

int CommissionItem::productId() const
{
    return productId_;
}

void CommissionItem::setProductId(int newProductId)
{
    productId_ = newProductId;
}

int CommissionItem::quantity() const
{
    return quantity_;
}

void CommissionItem::setQuantity(int newQuantity)
{
    quantity_ = newQuantity;
}

int CommissionItem::supplierId() const
{
    return supplierId_;
}

void CommissionItem::setSupplierId(int newSupplierId)
{
    supplierId_ = newSupplierId;
}

double CommissionItem::unitCommission() const
{
    return unitCommission_;
}

void CommissionItem::setUnitCommission(double newUnitCommission)
{
    unitCommission_ = newUnitCommission;
}

QUrl CommissionItem::imageUrl() const
{
    return imageUrl_;
}

void CommissionItem::setImageUrl(const QUrl &newImageUrl)
{
    imageUrl_ = newImageUrl;
}

} // namespace Billing
