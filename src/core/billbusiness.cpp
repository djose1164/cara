#include "billbusiness.h"
#include "constants.h"
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

void BillBusiness::fetchBillsByCustomerId(int customerId)
{
    Q_ASSERT(customerId);

    billRepository_->fetchByCustomerId(customerId);

    const auto repository{dynamic_cast<DataLayer::BillRepository *>(billRepository_)};

    connect(repository, &DataLayer::BillRepository::billsFetched, this, [&](const QByteArray &data) {
        const auto resp{QJsonDocument::fromJson(data).object()};
        const auto orders{resp["orders"].toArray()};

        if (orders.isEmpty())
        {
            emit ordersNotFound();
            return;
        }
        const auto ordersArr = resp["orders"].toArray();
        for (const auto &order : ordersArr)
        {
            const auto orderMap{order.toVariant().toMap()};
            Q_ASSERT(!orderMap.isEmpty());

            model_->append(BillEntity::fromMap(orderMap));
        }
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
        const auto doc = QJsonDocument::fromJson(data).object();
        const auto ordersArray = doc["orders"].toArray();
        for (const auto &order : ordersArray)
            model_->append(BillEntity::fromMap(order.toVariant().toMap()));
        emit populated();
    });
}

void BillBusiness::clearModel()
{
    model_->clear();
}
}
