#include "bill.h"

using namespace EntityLayer;

namespace Entity {
using Model::OrderModel;

Bill::Bill(QObject *parent)
    : QObject{parent}
    , orderId_{}
    , details_{new OrderModel(this)}
    , payment_{}
    , orderStatus_{nullptr}
    , date_{}
    , customer_{}
{
}

QDateTime Bill::date() const
{
    return date_.value();
}

void Bill::setDate(QDateTime date)
{
    date_ = date;
}

QBindable<QDateTime> Bill::bindableDate()
{
    return &date_;
}

Bill *Bill::fromMap(const QVariantMap &billMap)
{
    Q_ASSERT(!billMap.isEmpty());
    auto bill{new Bill};

    const auto strDate{billMap["date"].toString()};

    bill->setDate(QDateTime::fromString(strDate, "dd/MM/yyyy"));
    bill->setOrderId(billMap["id"].toInt());
    bill->setIsTaken(billMap["is_taken"].toBool());

    bill->setPayment(
        Entity::Payment::fromJsonObject(billMap["payment"].toJsonObject())
    );

    bill->setOrderStatus(
        OrderStatus::fromMap(billMap["order_status"].toMap())
    );

    bill->setCustomer(
        Entity::Customer::fromJson(billMap["customer"].toJsonObject())
    );

    const auto queueMap{billMap["queue"].toMap()};
    bill->setDone(queueMap["is_done"].toBool());

    for (const auto &detail : billMap["order_details"].toList())
    {
        const auto map{detail.toMap()};
        auto newDetail{new DetailsEntity};
        newDetail->setQuantity(map["quantity"].toInt());

        newDetail->setProduct(
            Product::fromMap(map["product"].toMap())
        );

        newDetail->setParent(bill);
        bill->appendDetail(newDetail);
    }

    return bill;
}

int Bill::orderId() const
{
    return orderId_.value();
}

void Bill::setOrderId(int orderId)
{
    orderId_ = orderId;
}

QBindable<int> Bill::bindableOrderId()
{
    return &orderId_;
}

void Bill::setDetailsModel(OrderModel * const model)
{
    details_ = model;
}

Entity::Payment Bill::payment() const
{
    return payment_;
}

void Bill::setPayment(const Entity::Payment &payment)
{
    payment_ = payment;
}

OrderModel *Bill::details() const
{
    return details_;
}

QByteArray Bill::toJson() const
{
    QJsonObject object{};
    object["customer_id"] = customer_.customerId();
    object["order_id"] = orderId();
    object["date"] = date().toString();

    QJsonArray array{};
    for (int i{}; i < details_->count(); ++i)
    {
        const auto detail{details_->get(i)};
        const auto detailJson{detail->toJson()};
        const auto doc{QJsonDocument::fromJson(detailJson)};
        array.append(doc.object());
    }
    object["details"] = array;

    const auto paymentObj{QJsonDocument::fromJson(payment_.toJson()).object()};
    object["payment"] = paymentObj;

    const auto doc{QJsonDocument(object)};
    return doc.toJson(QJsonDocument::Compact);
}

void Bill::appendDetail(DetailsEntity * const detail)
{
    Q_ASSERT(detail);

    details_->append(detail);
}

bool Bill::done() const
{
    return done_;
}

void Bill::setDone(bool newDone)
{
    done_ = newDone;
}

bool Bill::isTaken() const
{
    return isTaken_;
}

void Bill::setIsTaken(bool newIsTaken)
{
    isTaken_ = newIsTaken;
}

Entity::Customer Bill::customer() const
{
    return customer_;
}

void Bill::setCustomer(const Entity::Customer &newCustomer)
{
    customer_ = newCustomer;
}

OrderStatus *Bill::orderStatus() const
{
    return orderStatus_;
}

void Bill::setOrderStatus(OrderStatus *newOrderStatus)
{
    orderStatus_ = newOrderStatus;
    orderStatus_->setParent(this);
}
}

