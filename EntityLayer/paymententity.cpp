#include "paymententity.h"

using namespace EntityLayer;

PaymentEntity::PaymentEntity(QObject *parent)
    : QObject{parent}
    , paidAmount_{}
    , amountToPay_{}
{

}
