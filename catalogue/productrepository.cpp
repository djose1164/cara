
#include "productrepository.h"
#include "constants.h"
#include "currentuser.h"

namespace Catalogue {
ProductRepository::ProductRepository(QObject *parent)
    : QObject{parent}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{nullptr}
    , base_{BusinessLayer::Constants::instance()->base() + "/products"}
{

}

void ProductRepository::add(const QVariantMap &product)
{
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, "image/jpeg");

    const QUrl imgUrl{product.value("image_url").toUrl()};
    auto img{ new QFile(imgUrl.toLocalFile()) };
    imagePart.setHeader(
        QNetworkRequest::ContentDispositionHeader,
        QString("form-data; name=\"image\"; filename=\"%1\"")
            .arg(QFileInfo(*img).fileName())
    );

    QHttpPart jsonPart;
    jsonPart.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"jsonData\"");
    jsonPart.setBody(
        QJsonDocument::fromVariant(product)
            .toJson(QJsonDocument::Compact)
    );

    if (!img->open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open file for reading";
        emit errorOccurred("El archivo no existe.");
        return;
    }
    imagePart.setBodyDevice(img);

    auto multiPart{ new QHttpMultiPart(QHttpMultiPart::FormDataType) };

    img->setParent(multiPart);

    multiPart->append(imagePart);
    multiPart->append(jsonPart);

    const QUrl url{"http://127.0.0.1:5000/api/products/"};
    QNetworkRequest request{url};
    request.setRawHeader("Authorization", CurrentUser::instance()->accessToken().toUtf8());

    reply_ = manager_->post(request, multiPart);
    multiPart->setParent(reply_);

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

    const QString token{CurrentUser::instance()->accessToken()};
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
}
