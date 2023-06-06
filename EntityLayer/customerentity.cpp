#include "customerentity.h"

using EntityLayer::CustomerEntity;

CustomerEntity::CustomerEntity(QObject *parent)
    : UserEntity{parent}
    , bills_{new BillModel(this)}
{

}

void EntityLayer::CustomerEntity::setBills(BillModel * const bills)
{
    bills_ = bills;
}

ModelLayer::BillModel *EntityLayer::CustomerEntity::bills() const
{
    return bills_;
}
