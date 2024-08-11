#include "restpricehistoryrepository.h"
#include "Network/networkrequestsingleton.h"
#include "Network/qnamsingleton.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QRestReply>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;

namespace
{
constexpr auto priceHistoryLocator{"products/%1/price_history"_L1};
constexpr auto priceHistoryField{"price_history"_L1};
}

namespace Product
{

RestPriceHistoryRepository::RestPriceHistoryRepository(QObject *parent)
    : PriceHistoryRepository{parent}
    , manager_{new QRestAccessManager{QNAMSingleton::instance().qnam(), this}}
    , api_{Network::NetworkRequestSingleton::instance().api()}
    , priceType_{}
{
}

QBindable<QString> RestPriceHistoryRepository::bindablePriceType()
{
    return &priceType_;
}

void RestPriceHistoryRepository::addNewPrice(const QVariantMap &newPrice) noexcept
{
    const QString productId{newPrice["productId"_L1].toString()};
    manager_->post(api_->createRequest(priceHistoryLocator.arg(productId)),
                   newPrice,
                   this,
                   &RestPriceHistoryRepository::newPriceAdded);
}

void RestPriceHistoryRepository::fetchPriceHistoryBy(ProductId productId) noexcept
{
    QUrlQuery query;
    if (!(*priceType_).isEmpty())
        query.addQueryItem("price_type"_L1, "sell"_L1);
    manager_->get(api_->createRequest(priceHistoryLocator.arg(QString::number(productId)), query),
                  this,
                  &RestPriceHistoryRepository::priceHistoryFetched);
}

void RestPriceHistoryRepository::newPriceAdded(QRestReply &reply)
{
    if (!reply.isSuccess())
    {
        return;
    }

    if (const auto json{reply.readJson()}; json)
    {
        const QJsonDocument doc{(*json)[priceHistoryField].toObject()};
        emit added(doc.toJson());
    }
}

void RestPriceHistoryRepository::priceHistoryFetched(QRestReply &reply) noexcept
{
    if (const auto json{reply.readJson()};
        reply.isSuccess() && json && json->isObject())
    {
        const QJsonDocument doc{QJsonDocument{(*json)[priceHistoryField].toArray()}};
        emit fetched(doc.toJson());
    }
}

} // namespace Product
