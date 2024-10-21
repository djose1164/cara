#include "restsalerepository.h"
#include "Network/networkrequestsingleton.h"
#include "Network/pagination.h"
#include "Network/qnamsingleton.h"
#include "sale.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRestAccessManager>
#include <QRestReply>

using namespace Qt::StringLiterals;

namespace Billing
{

RestSaleRepository::RestSaleRepository(QObject *parent)
    : SaleRepository{parent}
    , api_{Network::NetworkRequestSingleton::instance().api()}
    , manager_{new QRestAccessManager(QNAMSingleton::instance().qnam(), this)}
    , path_{"sales"_L1}
    , pagination_{}
{
}

void RestSaleRepository::fetchSales()
{
    QUrlQuery query;
    pagination_.nextPage();
    query = pagination_.toUrlQuery();
    qDebug() << "RestSaleRepository::fetchSales: " << pagination_;
    const auto request{api_->createRequest(path_, query)};
    manager_->get(request, this, &RestSaleRepository::salesFetched);
}

void RestSaleRepository::addSale(const Sale &sale)
{
    manager_->post(api_->createRequest(path_),
                   QJsonDocument{sale.toJson()},
                   this,
                   [this](QRestReply &reply)
                   {
                       if (!reply.isSuccess())
                       {
                           emit errorOccurred(reply.readBody());
                           return;
                       }

                       if (const auto json{reply.readJson()};
                           json and json->isObject() and json->object().contains("sale"_L1))
                       {
                           emit added((*json)["sale"_L1].toObject());
                       }
                   });
}

void RestSaleRepository::setPagination(const Network::Pagination &newPagination)
{
    pagination_ = newPagination;
}

QList<Sale> RestSaleRepository::mapResponseToSales(const QJsonArray &arr)
{
    QList<Sale> saleList;
    for (const auto &item : arr)
    {
        saleList.append(Sale::fromJson(item.toObject()));
    }
    return saleList;
}

void RestSaleRepository::salesFetched(QRestReply &reply)
{
    if (!reply.isSuccess())
    {
        emit errorOccurred(reply.readBody());
        return;
    }

    constexpr auto sales{"sales"_L1};
    if (const auto json{reply.readJson()};
        json && json->isObject() && json->object().contains(sales))
    {
        if (!(*json)[sales].isArray())
        {
            emit errorOccurred("sales is not an array"_ba);
            return;
        }

        const auto saleList{mapResponseToSales((*json)[sales].toArray())};
        emit fetched(saleList,
                     Network::Pagination::fromJson(
                         (*json)["pagination"_L1].toObject()));
    }
}

} // namespace Billing
