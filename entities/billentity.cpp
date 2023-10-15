#include "billentity.h"

using EntityLayer::DetailsEntity, EntityLayer::BillEntity, ModelLayer::OrderModel;

BillEntity::BillEntity(QObject *parent)
    : QObject{parent}
    , orderId_{}
    , user_{new UserEntity(this)}
    , details_{new OrderModel(this)}
    , payment_{nullptr}
    , orderStatus_{nullptr}
    , date_{}
{
}

EntityLayer::UserEntity *EntityLayer::BillEntity::user() const
{
    return user_;
}

QDate EntityLayer::BillEntity::date() const
{
    return date_.value();
}

void EntityLayer::BillEntity::setDate(QDate date)
{
    date_ = date;
}

QBindable<QDate> EntityLayer::BillEntity::bindableDate()
{
    return &date_;
}

BillEntity *BillEntity::fromMap(const QVariantMap &billMap)
{
    auto bill{new BillEntity};

    const auto strDate{billMap["date"].toString()};

    bill->setDate(QDate::fromString(strDate, "dd/MM/yyyy"));
    bill->setOrderId(billMap["id"].toInt());

    bill->setPayment(
        PaymentEntity::fromMap(billMap["payment"].toMap())
    );

    bill->setOrderStatus(
        OrderStatus::fromMap(billMap["order_status"].toMap())
    );

    for (const auto &detail : billMap["order_details"].toList())
    {
        const auto map{detail.toMap()};
        auto newDetail{new EntityLayer::DetailsEntity};
        newDetail->setQuantity(map["quantity"].toInt());

        newDetail->setProduct(
            Product::fromMap(map["product"].toMap())
        );

        bill->appendDetail(newDetail);
    }

    return bill;
}

int EntityLayer::BillEntity::orderId() const
{
    return orderId_.value();
}

void EntityLayer::BillEntity::setOrderId(int orderId)
{
    orderId_ = orderId;
}

QBindable<int> EntityLayer::BillEntity::bindableOrderId()
{
    return &orderId_;
}

void EntityLayer::BillEntity::setDetailsModel(OrderModel * const model)
{
    details_ = model;
}

EntityLayer::PaymentEntity *EntityLayer::BillEntity::payment() const
{
    return payment_;
}

void EntityLayer::BillEntity::setPayment(PaymentEntity * const payment)
{
    payment_ = payment;
    payment_->setParent(this);
}

void EntityLayer::BillEntity::setUser(UserEntity * const user)
{
    user_ = user;
}

OrderModel *BillEntity::details() const
{
    return details_;
}

QByteArray EntityLayer::BillEntity::toJson() const
{
    QJsonObject object{};
    object["user_id"] = user_->userId();
    object["customer_id"] = user_->customerId();
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

void EntityLayer::BillEntity::appendDetail(DetailsEntity * const detail)
{
    Q_ASSERT(detail);

    details_->append(detail);
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

