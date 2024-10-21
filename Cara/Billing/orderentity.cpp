#include "orderentity.h"

using namespace Qt::StringLiterals;

namespace
{
constexpr auto dateTimeFormat{"yyyy-MM-dd hh:mm:ss"_L1};
}

namespace EntityLayer {
OrderEntity::OrderEntity(QObject *parent)
    : QObject{parent}
    , customer_{}
    , details_{}
    , payment_{}
    , orderDate_{QDateTime::currentDateTimeUtc()}
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

QDateTime EntityLayer::OrderEntity::orderDate() const
{
    return orderDate_.value();
}

void OrderEntity::setOrderDate(const QDateTime &newOrderDate)
{
    orderDate_ = newOrderDate;
}

QBindable<QDateTime> OrderEntity::bindableOrderDate()
{
    return &orderDate_;
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
    if (!customer_.valid())
        throw std::runtime_error{"Invalid user entity"};

    QJsonObject object{};
    object["customer_id"] = customer_.customerId();

    const auto detailsDoc{QJsonDocument::fromJson(details_)};
    QJsonArray arr{};
    unsigned amountToPay{};
    for (const auto &detail : detailsDoc.array())
    {
        const auto detailObj{detail.toObject()};
        const auto productId{detailObj["product"_L1].toObject()["product_id"_L1].toInt()};
        const int price{detailObj["product"_L1]["price_history"_L1]
                                    .toArray()
                                    .at(0)["price"_L1]
                                    .toInt()};

        qDebug() << detailObj["product"_L1]["price_history"_L1].toArray().at(0);
        qDebug() << detailObj;

        QJsonObject detailsObj{};
        detailsObj["product_id"] = productId;

        const auto quantity{detailObj["quantity"].toInt()};
        detailsObj["quantity"] = quantity;

        Q_ASSERT(price);
        Q_ASSERT(quantity);

        amountToPay += quantity*price;

        arr.append(detailsObj);
    }
    object["products"] = arr;
    object["date"] = orderDate().toString(dateTimeFormat);

    payment_.setAmountToPay(amountToPay);
    Q_ASSERT(amountToPay);
    payment_.decideStatus();
    const auto doc_{QJsonDocument::fromJson(payment_.toJson())};


    object["pay"] = QJsonValue::fromVariant(doc_);

    const auto doc{QJsonDocument(object)};
    return doc.toJson(QJsonDocument::Compact);
}
}
