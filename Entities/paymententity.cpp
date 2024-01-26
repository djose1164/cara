#include "paymententity.h"

using EntityLayer::PaymentEntity;

PaymentEntity::PaymentEntity(QObject *parent)
    : QObject{parent}
    , paymentId_{}
    , paidAmount_{}
    , amountToPay_{}
{

}

PaymentEntity *PaymentEntity::fromMap(const QVariantMap &map)
{
    const auto payment{new PaymentEntity};
    payment->setAmountToPay(map["amount_to_pay"].toInt());
    payment->setPaidAmount(map["paid_amount"].toInt());
    payment->setPaymentId(map["id"].toInt());

    return payment;
}

void PaymentEntity::setPaymentId(int id)
{
    if (paymentId_ != id)
    {
        paymentId_ = id;
        emit paymentIdChanged();
    }
}

int PaymentEntity::paymentId() const
{
    return paymentId_;
}

void PaymentEntity::setPaidAmount(int amount)
{
    if (paidAmount_ != amount)
    {
        paidAmount_ = amount;
        emit paidAmoutChanged();
        emit statusChanged();
    }
}

int PaymentEntity::paidAmount() const
{
    return paidAmount_;
}

void PaymentEntity::setAmountToPay(int amount)
{
    if (amountToPay_ != amount)
    {
        amountToPay_ = amount;
        emit amountToPayChanged();
        emit statusChanged();
    }
}

int PaymentEntity::amountToPay() const
{
    return amountToPay_;
}

PaymentEntity::Status PaymentEntity::status() const
{
    if (amountToPay_ == paidAmount_)
        return Status::Paid;
    else if (paidAmount_ && paidAmount_ < amountToPay_)
        return Status::Credited;
    else
        return Status::Unpaid;
}

QByteArray PaymentEntity::toJson() const
{
    QJsonObject object{};
    object["id"] = paymentId_;
    object["paid_amount"] = paidAmount_;
    object["amount_to_pay"] = amountToPay_;
    object["payment_status_id"] = int(status());
    const QJsonDocument doc{object};

    return doc.toJson(QJsonDocument::Compact);
}
