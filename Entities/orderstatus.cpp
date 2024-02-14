#include "orderstatus.h"

OrderStatus::OrderStatus(QObject *parent)
    : QObject{parent}
{

}

OrderStatus *OrderStatus::fromMap(const QVariantMap &map)
{
    const auto orderStatus{new OrderStatus};
    orderStatus->setStatusId(map["id"].toInt());
    orderStatus->setStatusString(map["status"].toString());

    return orderStatus;
}

int OrderStatus::statusId() const
{
    return statusId_.value();
}

void OrderStatus::setStatusId(int statusId)
{
    statusId_ = statusId;
}

QBindable<int> OrderStatus::bindableStatusId() const
{
    return &statusId_;
}

QString OrderStatus::statusString() const
{
    return statusString_.value();
}

void OrderStatus::setStatusString(const QString &status)
{
    statusString_ = status;
}

QBindable<QString> OrderStatus::bindableStatusString() const
{
    return &statusString_;
}
