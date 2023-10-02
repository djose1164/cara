#include "billbusiness.h"
#include "constants.h"
#include "currentuser.h"

using namespace EntityLayer;
using namespace ModelLayer;

namespace BusinessLayer {
BillBusiness::BillBusiness(QObject *parent)
    : QObject{parent}
    , rest_{new Rest(this)}
    , model_{nullptr}
{
    const QString base{Constants::instance()->base()};
    rest_->base(base + "/orders");
}

BillModel *BillBusiness::model() const
{
    return model_.value();
}

void BillBusiness::setModel(BillModel * const model)
{
    model_ = model;
}

QBindable<ModelLayer::BillModel *> BillBusiness::bindableModel()
{
    return &model_;
}

void BillBusiness::populate()
{
    const auto user{CurrentUser::instance()->user()};
    if (user->userType() != UserEntity::UserTypes::USER)
        return;

    const int customerId{user->customerId()};
    fetchBillsByCustomerId(customerId);
}

void BillBusiness::fetchBillsByCustomerId(int customerId)
{
    Q_ASSERT(customerId);

    fetch(QString("/?customer_id=%1").arg(customerId));

    connect(rest_, &Rest::succeeded, this, [&](const QVariantMap &resp) {
        for (const auto &order : resp["orders"].toList())
            model_->append(BillEntity::fromMap(order.toMap()));
        emit populated();
    });
}

void BillBusiness::fetchBillById(int orderId)
{
    Q_UNUSED(orderId)

    // FIXME: Implement me
}

void BillBusiness::fetchBillsByAdminId(int customerId)
{   
    fetch(QString("/?admin_id=%1").arg(customerId));

    connect(rest_, &Rest::succeeded, this, [&](const QVariantMap &resp) {
        for (const auto &order : resp["orders"].toList())
            model_->append(BillEntity::fromMap(order.toMap()));
        emit populated();
    });
}

void BillBusiness::clearModel()
{
    model_->clear();
}

void BillBusiness::fetch(QStringView arg)
{
    const auto user{CurrentUser::instance()};
    rest_->accessToken(user->accessToken());

    rest_->get(arg.toString());
}
}
