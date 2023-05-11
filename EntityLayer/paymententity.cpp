#include "paymententity.h"

using EntityLayer::PaymentEntity;

PaymentEntity::PaymentEntity(QObject *parent)
    : QObject{parent}
    , paidAmount_{}
    , amountToPay_{}
{

}

void PaymentEntity::setPaidAmount(int amount)
{
    if (paidAmount_ != amount)
    {
        paidAmount_ = amount;
        emit paidAmoutChanged();
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
    }
}

int PaymentEntity::amountToPay() const
{
    return amountToPay_;
}

QByteArray PaymentEntity::toJson() const
{
    QJsonObject object{};
    object["paid_amount"] = paidAmount_;
    QJsonDocument doc{object};

    return doc.toJson(QJsonDocument::Compact);
}
