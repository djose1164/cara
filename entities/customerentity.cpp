#include "customerentity.h"

using EntityLayer::CustomerEntity;

CustomerEntity::CustomerEntity(QObject *parent)
    : UserEntity{parent}
    , bills_{new BillModel(this)}
    , adminId_{}
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

int CustomerEntity::adminId() const
{
    return adminId_;
}

void CustomerEntity::setAdminId(int adminId)
{
    adminId_ = adminId;
}

QBindable<int> CustomerEntity::bindableAdminId()
{
    return &adminId_;
}
