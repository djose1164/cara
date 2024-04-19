#include "productrepository.h"
#include "networkrequestsingleton.h"
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QFile>
#include <QFileInfo>

using namespace Qt::Literals::StringLiterals;

namespace
{
constexpr auto resourceID{"/%1"_L1};
constexpr auto pictureEndpoint{"/picture"_L1};
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
        auto tmp{product};
        const auto imagesVariant{tmp.take("images")};
        qDebug() << "ProductRepository::add: " << imagesVariant.value<QList<QUrl>>();
        qDebug() << "ProductRepository::add: " << imagesVariant;
        prepareImagePart(imagesVariant.toList());
        prepareJsonPart(tmp);

        reply_ = manager_->post(requestWithBoundary(), multiPart_);
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
        qDebug() << "ProductRepository::add: " << err.what();
        emit errorOccurred(err.what());
    }
}

QNetworkReply *ProductRepository::read(ProductId id)
{
    return manager_->get(
        api_->createRequest(path_ + resourceID.arg(QString::number(id)))
    );
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

QNetworkReply *ProductRepository::addPicture(ProductId id, const QVariantList &pictureList)
{
    multiPart_ = new QHttpMultiPart(QHttpMultiPart::FormDataType, this);
    prepareImagePart(pictureList);

    reply_ = manager_->post(
        requestWithBoundary(
            resourceID.arg(QString::number(id)) + pictureEndpoint
        ),
        multiPart_
    );
    multiPart_->setParent(reply_);

    return reply_;
}

QNetworkReply *ProductRepository::deletePicture(PictureId pictureId)
{
    return manager_->deleteResource(
        api_->createRequest(
            path_ + pictureEndpoint + resourceID.arg(QString::number(pictureId))
        )
    );
}

void ProductRepository::prepareImagePart(const QVariantList &pictureList)
{
    qDebug() << "ProductRepository::prepareImagePart: " << pictureList;
    if (pictureList.isEmpty())
        throw std::runtime_error{"Picture list cannot be empty."};

    for (const auto &picture : pictureList)
    {
        QHttpPart imagePart;
        imagePart.setHeader(QNetworkRequest::ContentTypeHeader, "image/jpeg"_L1);

        const QUrl imageUrl{picture.toMap()["image_url"].toString()};
#ifdef Q_OS_ANDROID
        const auto img{new QFile(imageUrl.toString())};
#else
        const auto img{new QFile(imageUrl.toLocalFile())};
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
            throw std::runtime_error(errorString_.toUtf8());
        }

        imagePart.setBodyDevice(img);
        img->setParent(multiPart_);

        multiPart_->append(imagePart);
    }
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

QNetworkRequest ProductRepository::requestWithBoundary(QAnyStringView endpoint) const
{
    QNetworkRequest request{api_->createRequest(path_ + endpoint.toString())};
    const auto headerValue{"multipart/form-data; boundary=\"%1\""_L1.arg(multiPart_->boundary())};
    request.setHeader(QNetworkRequest::ContentTypeHeader, headerValue);
    return request;
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
