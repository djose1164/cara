#include "sellerproductrepositoryrest.h"
#include "qnamsingleton.h"


using namespace Qt::StringLiterals;

namespace
{
constexpr auto resourceIdentification{"/%1"_L1};
constexpr auto inventoriesField{"inventories"_L1};
}

namespace ProductInventory {

SellerProductRepositoryREST::SellerId SellerProductRepositoryREST::sellerId() const
{
    return sellerId_;
}

void SellerProductRepositoryREST::setSellerId(SellerId newSellerId)
{
    if (sellerId_ == newSellerId)
        return;
    sellerId_ = newSellerId;
    emit sellerIdChanged();
}

SellerProductRepositoryREST::SellerProductRepositoryREST(QObject *parent)
    : IProductRepository{parent}
{
    setPath("inventory/"_L1);
}

void SellerProductRepositoryREST::add(const EntityLayer::Product &product)
{
}

void SellerProductRepositoryREST::update()
{
    qDebug() << "SellerProductRepositoryREST::update: updating...";
    const QString identifier{QString::number(sellerId_)};
    if (sellerId_ < 1)
    {
        const auto errStr{"Invalid sellerId: %1"_L1.arg(identifier)};
        qWarning() << "SellerProductRepositoryREST::update: " << errStr;
        emit errorOccurred(Error::FailedToUpdate, errStr);
        return;
    }

    const QUrlQuery query{ {"salesperson_id"_L1, identifier} };
    const auto req{api_->createRequest(path(), query)};

    const auto reply = manager_->get(req); // Reply is auto-deleted by manager.

    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        QByteArray &&data = reply->readAll();
        if (reply->error() != QNetworkReply::NoError)
        {
            emit errorOccurred(Error::FailedToFetch, reply->errorString());
            return;
        }

        const auto obj{QJsonDocument::fromJson(data).object()};
        const auto inventoryValue = obj.value("inventory");
        if (!inventoryValue.isArray())
        {
            qWarning() << "SellerProductRepositoryREST::getProductsBySellerId: inventory is not an array.";
            return;
        }

        const auto inventoryArray = inventoryValue.toArray();
        const auto productList{EntityLayer::Product::fromJsonArray(inventoryArray)};
        emit fetched(productList);
        qDebug() << "SellerProductRepositoryREST::update: updating...Done";
    });
}

void SellerProductRepositoryREST::remove(ProductId productId)
{
}

} // namespace ProductInventory

