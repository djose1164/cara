#include "inventoryrepository.h"
#include "networkrequestsingleton.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto resourceIdentification{"/%1"_L1};
}

namespace Data
{
InventoryRepository::InventoryRepository(QObject *parent)
    : Business::IInventoryRepository{parent}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{}
    , path_{"inventory/"_L1}
    , api_{Authentication::NetworkRequestSingleton::instance().api()}
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
        QList<EntityLayer::Product> products;
        for (const auto product : std::as_const(inventoryArray))
        {
            const auto productJson = QJsonDocument(product.toObject()).toJson();
            products.append(EntityLayer::Product::fromJson(productJson));
        }

        emit fetchedAll(products);
    });
}

void InventoryRepository::update(const QVariantMap &stocks)
{

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
}
