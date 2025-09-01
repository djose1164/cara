#include "billbusiness.h"
#include "billrepository.h"

using namespace Qt::StringLiterals;

namespace
{
constexpr auto ordersField{"orders"_L1};
}

namespace BusinessLayer
{
BillBusiness::BillBusiness(QObject *parent)
    : Billing::QueueRepository{parent}
    , billRepository_{new DataLayer::BillRepository(this)}
    , reply_{}
    , model_{nullptr}
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
        const auto ordersArray = doc[ordersField].toArray();
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

void BillBusiness::fetchOrdersInQueue(SalespersonId spId)
{
    if (spId < -1 or spId == 0)
    {
        const auto errorString{QStringLiteral("BillBusiness::fetchOrdersInQueue: Invalid spId: %1")
                                   .arg(QString::number(spId))};
        qDebug() << errorString;
        emit errorOccurred(errorString);
        return;
    }

    qDebug() << QStringLiteral("BillBusiness::fetchOrdersInQueue: spId: ") << spId << " fetching...";
    reply_ = billRepository_->fetchOrdersInQueue(spId);

    connect(reply_, &QNetworkReply::finished, this, [this] {
        auto reply{QRestReply{reply_}};
        if (!reply.isSuccess())
        {
            emit errorOccurred(reply.errorString());
            return;
        }

        if (const auto json{reply.readJson()};
            json && json->isObject() && json->object().contains(ordersField))
        {
            const auto orders = (*json)[ordersField].toArray();
            emit queueFetched(orders);
        }
    });
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
