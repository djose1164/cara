#include "productrepository.h"
#include "constants.h"
#include "tokenmanager.h"
#include <QHttpMultiPart>

namespace Catalogue {
ProductRepository::ProductRepository(QObject *parent)
    : QObject{parent}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{nullptr}
    , base_{}
    , multiPart_{nullptr}
{
    QQmlEngine engine;
    const auto constants{engine.singletonInstance<DataLayer::Constants *>("data", "Constants")};
    base_ = constants->base() + "/products";
}

void ProductRepository::add(const QVariantMap &product)
{
    multiPart_ = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    prepareImagePart(product.value("image_url").toUrl());
    prepareJsonPart(product);

    const QUrl url{base_ + "/products/"};
    QNetworkRequest request{url};
    request.setRawHeader("Authorization", DataLayer::TokenManager::instance()->accessToken().toUtf8());

    reply_ = manager_->post(request, multiPart_);
    multiPart_->setParent(reply_);

    connect(reply_, &QNetworkReply::finished, this, [&] {
        const auto content{reply_->readAll()};
        if (reply_->error() == QNetworkReply::NoError)
            emit added(content);
        else
            emit errorOccurred(content);

        reply_->deleteLater();
    });
}

void ProductRepository::update(const QVariantMap &product)
{
    QVariantMap productTmp{product};
    const QUrl url{base_ + "/" + productTmp["id"].toString()};
    productTmp.remove("id");

    const QString token{DataLayer::TokenManager::instance()->accessToken()};
    QNetworkRequest request{url};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", token.toUtf8());

    const auto payload = QJsonDocument::fromVariant(productTmp)
        .toJson(QJsonDocument::Compact);
    reply_ = manager_->put(request, payload);

    connect(reply_, &QNetworkReply::finished, this, [&] {
        if (reply_->error() == QNetworkReply::NoError)
            emit updated();

        reply_->deleteLater();
    });
}

void ProductRepository::fetchAll()
{
    QNetworkRequest request{QUrl(base_ + "/")};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    reply_ = manager_->get(request);

    connect(reply_, &QNetworkReply::finished, this, [&] {
        emit allFetched(reply_->readAll());

        reply_->deleteLater();
    });
}

void ProductRepository::prepareImagePart(const QUrl imageUrl)
{
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, "image/jpeg");

    auto img{ new QFile(imageUrl.toLocalFile()) };
    imagePart.setHeader(
        QNetworkRequest::ContentDispositionHeader,
        QString("form-data; name=\"image\"; filename=\"%1\"")
            .arg(QFileInfo(*img).fileName())
    );

    if (!img->open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open file for reading";
        emit errorOccurred("El archivo no existe.");
        return;
    }

    imagePart.setBodyDevice(img);
    img->setParent(multiPart_);

    multiPart_->append(imagePart);
}

void ProductRepository::prepareJsonPart(const QVariantMap &product)
{
    QHttpPart jsonPart;
    jsonPart.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"jsonData\"");
    jsonPart.setBody(
        QJsonDocument::fromVariant(product)
            .toJson(QJsonDocument::Compact)
    );

    multiPart_->append(jsonPart);
}
}
