#include "paymententity.h"

using EntityLayer::PaymentEntity;

PaymentEntity::PaymentEntity(QObject *parent)
    : QObject{parent}
    , paymentId_
    , paidAmount_{}
    , amountToPay_{}
{

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
        emit isPaidChanged();
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
        emit isPaidChanged();
    }
}

int PaymentEntity::amountToPay() const
{
    return amountToPay_;
}

bool PaymentEntity::isPaid() const
{
    return amountToPay_ == paidAmount_;
}

QByteArray PaymentEntity::toJson() const
{
    QJsonObject object{};
    object["paid_amount"] = paidAmount_;
    QJsonDocument doc{object};

    return doc.toJson(QJsonDocument::Compact);
}
