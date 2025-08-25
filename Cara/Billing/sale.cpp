#include "sale.h"

#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto idField{"id"_L1};
constexpr auto orderIdField{"order_id"_L1};
constexpr auto customerIdField{"customer_id"_L1};
constexpr auto totalAmountField{"total_amount"_L1};
constexpr auto createdAtField{"created_at"_L1};
constexpr auto saleItemsField{"sale_items"_L1};
}

namespace Billing
{

Sale::Sale()
    : saleId_{}
    , orderId_{}
    , customerId_{}
    , totalAmount_{}
    , saleItems_{}
    , createdAt_{}
{
}

int Sale::saleId() const
{
    return saleId_;
}

void Sale::setSaleId(int newSaleId)
{
    saleId_ = newSaleId;
}

int Sale::orderId() const
{
    return orderId_;
}

void Sale::setOrderId(int newOrderId)
{
    orderId_ = newOrderId;
}

double Sale::totalAmount() const
{
    return totalAmount_;
}

void Sale::setTotalAmount(double newTotalAmount)
{
    totalAmount_ = newTotalAmount;
}

QDateTime Sale::createdAt() const
{
    return createdAt_;
}

void Sale::setCreatedAt(const QDateTime &newCreatedAt)
{
    createdAt_ = newCreatedAt;
}

QJsonObject Sale::toJson() const
{
    return {
        {customerIdField, customerId_},
        {totalAmountField, totalAmount_},
        {orderIdField, orderId_}
    };
}

Sale Sale::fromJson(const QJsonObject &json)
{
    Sale sale;
    sale.setSaleId(json[idField].toInt());
    sale.setOrderId(json[orderIdField].toInt());
    sale.setCustomerId(json[customerIdField].toInt());
    sale.setTotalAmount(json[totalAmountField].toString().toDouble());
    // const QString createdAtString{json[createdAtField].toString()};
    // auto dateTime{QDateTime::fromString(createdAtString, Qt::ISODate)};
    // dateTime.setTimeZone(QTimeZone{Qt::UTC});
    sale.setCreatedAt(QDateTime::fromString(json[createdAtField].toString(),
                                            Qt::ISODate));
    sale.createdAt_.setTimeZone(QTimeZone{QTimeZone::UTC});

    QList<SaleItem> salesList;
    const auto saleItems = json[saleItemsField].toArray();
    for (const auto &itemValue : saleItems)
    {
        salesList.append(SaleItem::fromJson(itemValue.toObject()));
    }
    sale.saleItems_ = std::move(salesList);
    return sale;
}

int Sale::customerId() const
{
    return customerId_;
}

void Sale::setCustomerId(int newCustomerId)
{
    customerId_ = newCustomerId;
}

QList<SaleItem> Sale::saleItems() const
{
    return saleItems_;
}

void Sale::setSaleItems(const QList<SaleItem> &newSaleItems)
{
    saleItems_ = newSaleItems;
}

bool Sale::hasItems() const
{
    return saleItems_.count() > 0;
}

SaleItem::SaleItem()
    : productId_{}
    , quantity_{}
{
}

int SaleItem::productId() const
{
    return productId_;
}

void SaleItem::setProductId(int newProductId)
{
    productId_ = newProductId;
}

int SaleItem::quantity() const
{
    return quantity_;
}

void SaleItem::setQuantity(int newQuantity)
{
    quantity_ = newQuantity;
}

QJsonObject SaleItem::toJson() const
{
    return {{"qty"_L1, quantity_}, {"product_id", productId_}};
}

bool SaleItem::isValid() const
{
    return productId_ > 0 and quantity_ > 0;
}

SaleItem SaleItem::fromJson(const QJsonObject &json)
{
    SaleItem si;
    si.setProductId(json["product_id"_L1].toInt());
    si.setQuantity(json["quantity"_L1].toInt());
    return si;
}

} // namespace Billing
