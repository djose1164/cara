#include "orderentity.h"

using EntityLayer::DetailsEntity, EntityLayer::OrderEntity;

OrderEntity::OrderEntity(QObject *parent)
    : QObject{parent}
    , user_{new UserEntity(this)}
    , details_{}
    , payment_{new PaymentEntity(this)}
{
}

EntityLayer::UserEntity *EntityLayer::OrderEntity::user() const
{
    return user_;
}

void EntityLayer::OrderEntity::setDetails(const QByteArray &details)
{
    details_ = details;
}

QBindable<QByteArray> EntityLayer::OrderEntity::bindableDetails()
{
    return &details_;
}

QByteArray EntityLayer::OrderEntity::details() const
{
    return details_.value();
}

void EntityLayer::OrderEntity::setUser(UserEntity * const user)
{
    if (user_ != user)
    {
        user_ = user;
        emit userChanged();
    }
}

QByteArray EntityLayer::OrderEntity::toJson() const
{
    QJsonObject object{};
    object["user_id"] = user_->userId();
    object["customer_id"] = user_->customerId();

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
    object["details"] = arr;

    payment_->setAmountToPay(amountToPay);
    const auto doc_{QJsonDocument::fromJson(payment_->toJson())};
    object["payment"] = QJsonValue::fromVariant(doc_);

    qDebug() << "OrderEntity::toJson " << object;

    const auto doc{QJsonDocument(object)};
    return doc.toJson(QJsonDocument::Compact);
}
