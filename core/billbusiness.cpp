#include "billbusiness.h"
#include "constants.h"
#include "currentuser.h"
#include "tokenmanager.h"
#include "billrepository.h"

using namespace EntityLayer;
using namespace ModelLayer;

namespace BusinessLayer {
BillBusiness::BillBusiness(QObject *parent)
    : QObject{parent}
    , rest_{new Rest(this)}
    , model_{nullptr}
    , billRepository_{new DataLayer::BillRepository(this)}
{
    QQmlEngine engine;
    const auto constants{engine.singletonInstance<DataLayer::Constants *>("data", "Constants")};
    const QString base{constants->base()};
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
    const auto user{BusinessLayer::CurrentUser::instance()->user()};
    if (user->userType() != UserEntity::UserTypes::USER)
        return;

    const int customerId{user->customerId()};
    fetchBillsByCustomerId(customerId);
}

void BillBusiness::fetchBillsByCustomerId(int customerId)
{
    Q_ASSERT(customerId);

    billRepository_->fetchByCustomerId(customerId);

    const auto repository{dynamic_cast<DataLayer::BillRepository *>(billRepository_)};

    connect(repository, &DataLayer::BillRepository::billsFetched, this, [&](const QByteArray &data) {
        const auto resp{QJsonDocument::fromJson(data).object()};
        for (const auto &order : resp["orders"].toVariant().toList())
            model_->append(BillEntity::fromMap(order.toMap()));
        emit populated();
    });
}

void BillBusiness::fetchBillById(int orderId)
{
    Q_UNUSED(orderId)

    // FIXME: Implement me
}

void BillBusiness::fetchBillsByAdminId(int adminId)
{
    billRepository_->fetchByAdminId(adminId);

    const auto repository{dynamic_cast<DataLayer::BillRepository *>(billRepository_)};

    connect(repository, &DataLayer::BillRepository::billsFetched, this, [&](const QByteArray &data) {
        auto doc{QJsonDocument::fromJson(data).object()};
        for (const auto &order : doc["orders"].toArray())
            model_->append(BillEntity::fromMap(order.toVariant().toMap()));
        emit populated();
    });
}

void BillBusiness::clearModel()
{
    model_->clear();
}
}
