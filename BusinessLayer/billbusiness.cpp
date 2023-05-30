#include "billbusiness.h"
#include "constants.h"
#include "currentuser.h"

using BusinessLayer::BillBusiness, EntityLayer::UserEntity,
    ModelLayer::BillModel;

BillBusiness::BillBusiness(QObject *parent)
    : QObject{parent}
    , rest_{new Rest(this)}
    , model_{nullptr}
{
    const QString base{Constants::instance()->base()};
    rest_->base(base + "/customers");

    QSettings settings{};
    const QString token{settings.value("Credentials/refreshToken").toString()};
    rest_->refreshToken(token);
    rest_->refreshAccessToken();

    connect(rest_, &Rest::accessTokenChanged, this, [&] {
        emit completed();
    });
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
    const int customerId{CurrentUser::instance()->customerId()};
    fetchBillsByCustomerId(customerId);

    connect(rest_, &Rest::succeeded, this, [&] {
        emit populated();
    });
}

void BillBusiness::fetchBillsByCustomerId(int customerId)
{
    const QString base{Constants::instance()->base()};
    rest_->base(base + "/customers");

    rest_->get(QString("/%1").arg(customerId));

    connect(rest_, &Rest::succeeded, this, [&](const QVariantMap &resp) {
        const auto customer{resp["customer"].toMap()};
        for (const auto &order : customer["orders"].toList())
            model_->append(BillEntity::fromMap(order.toMap()));
    });
}

void BillBusiness::fetchBillById(int orderId)
{
    Q_UNUSED(orderId)

    // FIXME: Implement me
}

void BillBusiness::clearModel()
{
    model_->clear();
}
