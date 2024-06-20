#include "inventoryrepository.h"
#include "Network/networkrequestsingleton.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto resourceIdentification{"/%1"_L1};
constexpr auto inventoriesField{"inventories"_L1};
}

namespace Data
{

InventoryRepository::InventoryRepository(QObject *parent)
    : Business::IInventoryRepository{parent}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{}
    , path_{"inventory/"_L1}
    , api_{Network::NetworkRequestSingleton::instance().api()}
{

}

void InventoryRepository::fetchAll(SalespersonId spId)
{
    const QUrlQuery query{ {"salesperson_id"_L1, QString::number(spId)} };
    const auto req{api_->createRequest(path_, query)};

    reply_ = manager_->get(req);

    connect(reply_, &QNetworkReply::finished, this, [&] {
        setData(reply_->readAll());
        if (reply_->error() != QNetworkReply::NoError)
        {
            setErrorString(reply_->errorString());
            emit errorOccurred(errorString());
            return;
        }

        const auto obj{QJsonDocument::fromJson(data()).object()};
        const auto inventoryValue = obj.value("inventory");
        if (!inventoryValue.isArray())
        {
            qWarning() << "InventoryRepository::fetchAll: inventory is not an array.";
            return;
        }

        const auto inventoryArray = inventoryValue.toArray();

        emit fetchedAll(mapJsonToProducts(inventoryArray));
    });
}

void InventoryRepository::update(const QVariantMap &stocks)
{

}

void InventoryRepository::fetchAnyByProductId(ProductId productId)
{
    const QUrlQuery query{ {"product_id"_L1, QString::number(productId)} };
    const auto req{api_->createRequest(path_, query)};

    reply_ = manager_->get(req);

    connect(reply_, &QNetworkReply::finished, this, [this] {
        reply_->deleteLater();

        QRestReply reply{reply_};

        if (!reply.isSuccess())
        {
            setErrorString(reply.errorString());
            emit errorOccurred(errorString_);
            return;
        }

        if (const auto json{reply.readJson()};
            json and json->isObject() and json->object().contains(inventoriesField))
        {
            setData(json->toJson());
            emit anyFetched(mapJsonToProducts((*json)[inventoriesField].toArray()));
        }
    });
}

QList<EntityLayer::Product> InventoryRepository::mapJsonToProducts(const QJsonArray &array) const
{
    QList<EntityLayer::Product> productList{array.count()};

    std::ranges::transform(array, productList.begin(), [](const QJsonValue &jsonValue) {
        auto &&doc{QJsonDocument{jsonValue.toObject()}};
        return EntityLayer::Product::fromJson(doc.toJson());
    });

    return productList;
}

QString InventoryRepository::errorString() const
{
    return errorString_;
}

void InventoryRepository::setErrorString(const QString &newErrorString)
{
    errorString_ = newErrorString;
}

QByteArray InventoryRepository::data() const
{
    return data_;
}

void InventoryRepository::setData(const QByteArray &newData)
{
    data_ = newData;
}

QBindable<QByteArray> InventoryRepository::bindableData() const
{
    return &data_;
}

}
