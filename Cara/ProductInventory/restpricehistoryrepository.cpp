#include "restpricehistoryrepository.h"
#include "qnamsingleton.h"
#include "networkrequestsingleton.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QRestReply>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;

namespace
{
constexpr auto priceHistoryLocator{"products/%1/price_history"_L1};
constexpr auto priceHistoryField{"price_history"_L1};
constexpr auto productIdField{"productId"_L1};
constexpr auto priceTypeField{"priceType"_L1};
constexpr auto supplierIdField{"supplierId"_L1};
}

namespace ProductInventory
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
    try
    {
        validateMap(newPrice);
        const QString productId{newPrice["productId"_L1].toString()};
        manager_->post(api_->createRequest(priceHistoryLocator.arg(productId)),
                       newPrice,
                       this,
                       &RestPriceHistoryRepository::newPriceAdded);
    }
    catch (const std::invalid_argument &err)
    {
        qWarning() << "RestPriceHistoryRepository::addNewPrice: " << err.what();
        emit errorOccurred(Error::FailedToAddNewPrice, err.what());
    }
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
        const auto json{reply.readJson()};
        emit errorOccurred(Error::FailedToAddNewPrice,
                           (*json)["error"_L1].toString());
        return;
    }

    if (const auto json{reply.readJson()};
        json and json->isObject() and json->object().contains(priceHistoryField))
    {
        qDebug() << *json;
        const QJsonDocument doc{(*json)[priceHistoryField].toObject()};
        emit added(doc.toJson());
    }
    else
    {
        emit errorOccurred(Error::UnknownError,
                           "%1 is not present in response."_L1
                                .arg(priceHistoryField));
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

void RestPriceHistoryRepository::validateMap(const QVariantMap &map)
{
    constexpr auto containsMsg = [](QAnyStringView message) {
        return "map doesn't contain '%1' key."_L1
                                                .arg(message.toString())
                                                .toUtf8();
    };

    constexpr auto invalidMsg = [](QAnyStringView key, QAnyStringView value) {
        return "Invalid '%1': %2."_L1
                                    .arg(key.toString())
                                    .arg(value.toString())
                                    .toUtf8();
    };

    if (not map.contains(productIdField))
        throw std::invalid_argument{containsMsg(productIdField)};
    if (map[productIdField].toInt() < 1)
        throw std::invalid_argument{invalidMsg(productIdField, map[productIdField].toString())};

    if (not map.contains(priceTypeField))
        throw std::invalid_argument{containsMsg(priceTypeField)};
    if (QString priceType{map[priceTypeField].toString()};
        not (priceType == "sell"_L1 or priceType == "buy"_L1))
    {
        throw std::invalid_argument{invalidMsg(priceTypeField, priceType)};
    }

    if (map.contains(supplierIdField) and map[supplierIdField].toInt() < 1)
    {
        throw std::invalid_argument{invalidMsg(supplierIdField,
                                               map[supplierIdField].toString())};
    }
}

} // namespace ProductInventory
