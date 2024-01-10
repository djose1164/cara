#include "inventoryrepository.h"
#include "localuserstore.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace Data
{
InventoryRepository::InventoryRepository(QObject *parent)
    : Business::IInventoryRepository{parent}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{}
{

}

void InventoryRepository::fetchAll(SalespersonId spId)
{
    const auto req{prepareRequest(QString("/?salesperson_id=%1").arg(spId))};

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
        QList<QPointer<EntityLayer::Product>> products;
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

QNetworkRequest InventoryRepository::prepareRequest(const QString &endpoint)
{
    const QUrl url{QSettings().value("base").toString() + "/inventory" + endpoint};
    QNetworkRequest req{url};
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    const QString token{Business::LocalUserStore().user().accessToken()};
    req.setRawHeader("Authorization", token.toUtf8());
    return req;
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
