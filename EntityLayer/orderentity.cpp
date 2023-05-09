#include "orderentity.h"

using EntityLayer::DetailsEntity, EntityLayer::OrderEntity;

OrderEntity::OrderEntity(QObject *parent)
    : QObject{parent}
    , user_{new UserEntity(this)}
    , details_{}
    , payment_{new PaymentEntity(this)}
{
}

QList<DetailsEntity *> OrderEntity::details() const
{
    return details_;
}

DetailsEntity *OrderEntity::detail(qsizetype index) const
{
    return details_[index];
}

void OrderEntity::appendDetail(DetailsEntity *detail)
{
    qDebug() << "C++: detail: " << detail;
    details_.append(detail);
}

void OrderEntity::clear()
{
    details_.clear();
}

void OrderEntity::replaceDetail(qsizetype index, DetailsEntity *detail)
{
    details_.replace(index, detail);
}

void OrderEntity::popBack()
{
    details_.pop_back();
}

EntityLayer::UserEntity *EntityLayer::OrderEntity::user() const
{
    return user_;
}

void EntityLayer::OrderEntity::setDetails(const QList<DetailsEntity *> &details)
{
    if (details_ != details)
    {
        details_ = details;
        emit detailsChanged();
    }
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

    QJsonArray array{};
    foreach (const auto detail, details_)
    {
        const auto detailJson{detail->toJson()};
        const auto doc{QJsonDocument::fromJson(detailJson)};
        array.append(QJsonValue::fromVariant(doc));
    }
    object["details"] = array;

    const auto doc_{QJsonDocument::fromJson(payment_->toJson())};
    object["payment"] = QJsonValue::fromVariant(doc_);

    qDebug() << "OrderEntity::toJson " << object;

    const auto doc{QJsonDocument(object)};
    return doc.toJson(QJsonDocument::Compact);
}
