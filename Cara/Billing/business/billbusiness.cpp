#include "billbusiness.h"
#include "billrepository.h"

namespace BusinessLayer
{
BillBusiness::BillBusiness(QObject *parent)
    : QObject{parent}
    , reply_{}
    , model_{nullptr}
    , billRepository_{new DataLayer::BillRepository(this)}
{
}

BillModel *BillBusiness::model() const
{
    return model_.value();
}

void BillBusiness::setModel(BillModel * const model)
{
    model_ = model;
}

QBindable<BillModel *> BillBusiness::bindableModel()
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
        const auto ordersValue{resp["orders"]};
        if (!ordersValue.isArray())
        {
            emit ordersNotFound();
            return;
        }

        const auto ordersArr = resp["orders"].toArray();
        if (ordersArr.isEmpty())
        {
            emit ordersNotFound();
            return;
        }

        model_->clear();
        for (const auto &order : std::as_const(ordersArr))
        {
            const auto orderMap{order.toVariant().toMap()};
            model_->append(Entity::Bill::fromMap(orderMap));
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
            model_->append(Entity::Bill::fromMap(order.toVariant().toMap()));
        emit populated();
    });
}

void BillBusiness::fetchUnpaidBills()
{
    qDebug() << QStringLiteral("BillBusiness::fetchUnpaidBills: fetching...");
    reply_ = billRepository_->fetchUnpaidBills();

    connect(reply_, &QNetworkReply::finished, this, &BillBusiness::handleRequestFinished);
}

void BillBusiness::fetchOrdersInQueue()
{
    qDebug() << QStringLiteral("BillBusiness::fetchOrdersInQueue: fetching...");
    reply_ = billRepository_->fetchOrdersInQueue();

    connect(reply_, &QNetworkReply::finished, this, &BillBusiness::handleRequestFinished);
}

void BillBusiness::clearModel()
{
    model_->clear();
}

void BillBusiness::handleRequestFinished()
{
    if (reply_->error() != QNetworkReply::NoError)
    {
        emit ordersNotFound();
        qDebug() << QStringLiteral("BillBusiness::handleRequestFinished: no orders found");
        qDebug() << QStringLiteral("BillBusiness::handleRequestFinished: %1").arg(reply_->errorString());
        return;
    }

    const auto data{reply_->readAll()};
    const auto resp{QJsonDocument::fromJson(data).object()};
    const auto ordersValue{resp["orders"]};
    if (!ordersValue.isArray())
    {
        emit ordersNotFound();
        return;
    }

    const auto ordersArr = resp["orders"].toArray();
    if (ordersArr.isEmpty())
    {
        emit ordersNotFound();
        return;
    }

    for (const auto &order : std::as_const(ordersArr))
    {
        const auto orderMap{order.toVariant().toMap()};
        model_->append(Entity::Bill::fromMap(orderMap));
    }
    emit populated();
    qDebug() << QStringLiteral("BillBusiness::handleRequestFinished: Done");

    reply_->deleteLater();
}
}
