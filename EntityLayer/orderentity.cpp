#include "orderentity.h"

using EntityLayer::DetailsEntity, EntityLayer::OrderEntity;

OrderEntity::OrderEntity(QObject *parent)
    : QObject{parent}
    , customerId_{}
    , userId_{}
    , date_{}
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

int EntityLayer::OrderEntity::customerId() const
{
    return customerId_;
}

int EntityLayer::OrderEntity::userId() const
{
    return userId_;
}

QDateTime EntityLayer::OrderEntity::date() const
{
    return date_;
}

void EntityLayer::OrderEntity::setDetails(const QList<DetailsEntity *> &details)
{
    if (details_ != details)
    {
        details_ = details;
        emit detailsChanged();
    }
}
