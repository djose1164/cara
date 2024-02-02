#include "productrepository.h"
#include "networkrequestsingleton.h"
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QFile>
#include <QFileInfo>

using namespace Qt::Literals::StringLiterals;

namespace
{
constexpr auto resourceID{"%1"_L1};
}

namespace Catalogue {
ProductRepository::ProductRepository(QObject *parent)
    : QObject{parent}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{nullptr}
    , multiPart_{nullptr}
    , path_{"products"_L1}
    , errorString_{}
    , api_{Utils::NetworkRequestSingleton::instance().api()}
{
}

void ProductRepository::add(const QVariantMap &product)
{
    try
    {
        multiPart_ = new QHttpMultiPart(QHttpMultiPart::FormDataType, this);
        const auto tmp{product};
        prepareImagePart(tmp.value("image_url").toUrl());
        prepareJsonPart(tmp);

        QNetworkRequest request{api_->createRequest("products/"_L1)};
        const auto headerValue{"multipart/form-data; boundary=\"%1\""_L1.arg(multiPart_->boundary())};
        request.setHeader(QNetworkRequest::ContentTypeHeader, headerValue);

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
    catch(const std::runtime_error &err)
    {
        emit errorOccurred(err.what());
    }
}

void ProductRepository::update(const QVariantMap &product)
{
    QVariantMap productTmp{product};
    const int productId{productTmp["id"].toInt()};
    productTmp.remove("id");

    const auto request{api_->createRequest(path_ + resourceID.arg(QString::number(productId)))};

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
    const auto request{api_->createRequest(path_ + "/"_L1)};

    reply_ = manager_->get(request);

    connect(reply_, &QNetworkReply::finished, this, [&] {
        emit allFetched(reply_->readAll());

        reply_->deleteLater();
    });
}

void ProductRepository::prepareImagePart(const QUrl imageUrl)
{
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, "image/jpeg"_L1);

#ifdef Q_OS_ANDROID
    auto img{ new QFile(imageUrl.toString()) };
#else
    auto img{ new QFile(imageUrl.toLocalFile()) };
#endif
    imagePart.setHeader(
        QNetworkRequest::ContentDispositionHeader,
        "form-data; name=\"image\"; filename=\"%1\""_L1
            .arg(QFileInfo(*img).fileName())
    );

    if (!img->open(QIODevice::ReadOnly))
    {
        qCritical() << "Failed to open file for reading";
        setErrorString("El archivo no existe."_L1);
        emit errorOccurred(errorString_.toUtf8());
        throw std::runtime_error(errorString_);
    }

    imagePart.setBodyDevice(img);
    img->setParent(multiPart_);

    multiPart_->append(imagePart);
}

void ProductRepository::prepareJsonPart(const QVariantMap &product)
{
    QHttpPart jsonPart;
    jsonPart.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"jsonData\""_L1);
    jsonPart.setBody(
        QJsonDocument::fromVariant(product)
            .toJson(QJsonDocument::Compact)
    );

    multiPart_->append(jsonPart);
}

QString ProductRepository::errorString() const
{
    return errorString_;
}

void ProductRepository::setErrorString(const QString &newErrorString)
{
    errorString_ = newErrorString;
}

QString ProductRepository::path() const
{
    return path_;
}

void ProductRepository::setPath(const QString &newPath)
{
    path_ = newPath;
}
}
