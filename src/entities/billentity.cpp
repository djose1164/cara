#include "billentity.h"
#include "userentity.h"

namespace EntityLayer {
using ModelLayer::OrderModel;

BillEntity::BillEntity(QObject *parent)
    : QObject{parent}
    , orderId_{}
    , user_{}
    , details_{new OrderModel(this)}
    , payment_{nullptr}
    , orderStatus_{nullptr}
    , date_{}
    , customer_{}
{
}

Entity::UserEntity BillEntity::user() const
{
    return user_;
}

QDateTime BillEntity::date() const
{
    return date_.value();
}

void BillEntity::setDate(QDateTime date)
{
    date_ = date;
}

QBindable<QDateTime> BillEntity::bindableDate()
{
    return &date_;
}

BillEntity *BillEntity::fromMap(const QVariantMap &billMap)
{
    Q_ASSERT(!billMap.isEmpty());
    auto bill{new BillEntity};

    const auto strDate{billMap["date"].toString()};

    bill->setDate(QDateTime::fromString(strDate, "dd/MM/yyyy"));
    bill->setOrderId(billMap["id"].toInt());

    bill->setPayment(
        PaymentEntity::fromMap(billMap["payment"].toMap())
    );

    bill->setOrderStatus(
        OrderStatus::fromMap(billMap["order_status"].toMap())
    );

    bill->setCustomer(
        Entity::Customer::fromJson(billMap["customer"].toJsonObject())
    );

    for (const auto &detail : billMap["order_details"].toList())
    {
        const auto map{detail.toMap()};
        auto newDetail{new DetailsEntity};
        newDetail->setQuantity(map["quantity"].toInt());

        newDetail->setProduct(
            Product::fromMap(map["product"].toMap())
        );

        bill->appendDetail(newDetail);
    }

    return bill;
}

int BillEntity::orderId() const
{
    return orderId_.value();
}

void BillEntity::setOrderId(int orderId)
{
    orderId_ = orderId;
}

QBindable<int> BillEntity::bindableOrderId()
{
    return &orderId_;
}

void BillEntity::setDetailsModel(OrderModel * const model)
{
    details_ = model;
}

PaymentEntity *BillEntity::payment() const
{
    return payment_;
}

void BillEntity::setPayment(PaymentEntity * const payment)
{
    payment_ = payment;
    payment_->setParent(this);
}

void BillEntity::setUser(Entity::UserEntity user)
{
    user_ = user;
}

OrderModel *BillEntity::details() const
{
    return details_;
}

QByteArray BillEntity::toJson() const
{
    QJsonObject object{};
    object["user_id"] = user_.userId();
    object["customer_id"] = user_.customerId();
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

    const auto paymentObj{QJsonDocument::fromJson(payment_->toJson()).object()};
    object["payment"] = paymentObj;

    const auto doc{QJsonDocument(object)};
    return doc.toJson(QJsonDocument::Compact);
}

void BillEntity::appendDetail(DetailsEntity * const detail)
{
    Q_ASSERT(detail);

    details_->append(detail);
}

Entity::Customer BillEntity::customer() const
{
    return customer_;
}

void BillEntity::setCustomer(const Entity::Customer &newCustomer)
{
    customer_ = newCustomer;
}

OrderStatus *BillEntity::orderStatus() const
{
    return orderStatus_;
}

void BillEntity::setOrderStatus(OrderStatus *newOrderStatus)
{
    orderStatus_ = newOrderStatus;
    orderStatus_->setParent(this);
}
}

