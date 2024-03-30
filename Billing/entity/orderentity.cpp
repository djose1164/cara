#include "orderentity.h"

namespace EntityLayer {
OrderEntity::OrderEntity(QObject *parent)
    : QObject{parent}
    , customer_{}
    , details_{}
    , payment_{}
{
}

Entity::Customer &OrderEntity::customer()
{
    return customer_;
}

void OrderEntity::setDetails(const QByteArray &details)
{
    details_ = details;
}

QBindable<QByteArray> OrderEntity::bindableDetails()
{
    return &details_;
}

QByteArray OrderEntity::details() const
{
    return details_.value();
}

void OrderEntity::setCustomer(const Entity::Customer &newCustomer)
{
    if (customer_ != newCustomer)
    {
        customer_ = newCustomer;
        emit customerChanged();
    }
}

QByteArray OrderEntity::toJson()
{
    QJsonObject object{};
    object["customer_id"] = customer_.customerId();

    const auto detailsDoc{QJsonDocument::fromJson(details_)};
    QJsonArray arr{};
    unsigned amountToPay{};
    for (const auto &detail : detailsDoc.array())
    {
        const auto detailObj{detail.toObject()};
        const auto productId{detailObj["product"].toObject()["product_id"].toInt()};
        const auto price{detailObj["product"].toObject()["price"].toInt()};

        QJsonObject detailsObj{};
        detailsObj["product_id"] = productId;

        const auto quantity{detailObj["quantity"].toInt()};
        detailsObj["quantity"] = quantity;

        amountToPay += quantity*price;

        arr.append(detailsObj);
    }
    object["products"] = arr;

    payment_.setAmountToPay(amountToPay);
    payment_.decideStatus();
    const auto doc_{QJsonDocument::fromJson(payment_.toJson())};


    object["pay"] = QJsonValue::fromVariant(doc_);

    const auto doc{QJsonDocument(object)};
    return doc.toJson(QJsonDocument::Compact);
}
}
