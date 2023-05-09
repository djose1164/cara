#include "paymententity.h"

using namespace EntityLayer;

PaymentEntity::PaymentEntity(QObject *parent)
    : QObject{parent}
    , paidAmount_{}
    , amountToPay_{}
{

}

QByteArray PaymentEntity::toJson() const
{
    QJsonObject object{};
    object["paid_amount"] = paidAmount_;
    QJsonDocument doc{object};

    return doc.toJson(QJsonDocument::Compact);
}
