#include "productrepository.h"
#include <QFile>
#include <QFileInfo>
#include <QHttpMultiPart>
#include <QJsonArray>
#include <QJsonDocument>
#include "Network/networkrequestsingleton.h"
#include "validationerror.h"

using namespace Qt::Literals::StringLiterals;

namespace
{
constexpr auto resourceID{"/%1"_L1};
constexpr auto pictureEndpoint{"/picture"_L1};
constexpr auto productsField{"products"_L1};
constexpr auto imageUrlField{"imageUrl"_L1};
}

namespace Catalogue {
ProductRepository::ProductRepository(QObject *parent)
    : QObject{parent}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{nullptr}
    , multiPart_{nullptr}
    , path_{"products"_L1}
    , errorString_{}
    , api_{Network::NetworkRequestSingleton::instance().api()}
{
    manager_->setAutoDeleteReplies(true);
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
    try
    {
        validateMap(product);
        QVariantMap productTmp{product};
        const int productId{productTmp["id"].toInt()};
        productTmp.remove("id");

        const auto request{api_->createRequest(path_ + resourceID.arg(QString::number(productId)))};

        const auto payload = QJsonDocument::fromVariant(productTmp).toJson(QJsonDocument::Compact);
        reply_ = manager_->put(request, payload);

        connect(reply_,
                &QNetworkReply::finished,
                this,
                [&]
                {
                    if (reply_->error() == QNetworkReply::NoError)
                        emit updated();

                    reply_->deleteLater();
                });
    }
    catch (const ValidationError &err)
    {
        qWarning() << "ProductRepository::update: " << err.what();
        emit errorOccurred(err.what());
    }
}

void ProductRepository::fetchAll()
{
    const auto request{api_->createRequest(path_ + "/"_L1)};

    reply_ = manager_->get(request);

    connect(reply_, &QNetworkReply::finished, this, [&] {
        auto &&data{reply_->readAll()};
        emit allFetched(data);

        if (const auto json{QJsonDocument::fromJson(data)};
            json.isObject() && json[productsField].isArray())
        {
            const auto productArray = json[productsField].toArray();
            QList<EntityLayer::Product> productList{productArray.size()};

            std::ranges::transform(productArray.begin(),
                                   productArray.end(),
                                   productList.begin(),
                                   [](const auto &jsonValue) -> EntityLayer::Product
                                   {
                                       return EntityLayer::Product::fromJson(QJsonDocument{jsonValue.toObject()}.toJson());
                                   });
            emit allProductsFetched(productList);

        }

        reply_->deleteLater();
    });
}

QNetworkReply *ProductRepository::addPicture(ProductId id, const QVariantList &pictureList)
{
    try
    {
        multiPart_ = new QHttpMultiPart(QHttpMultiPart::FormDataType, this);
        prepareImagePart(pictureList);

        const QString endpoint{resourceID.arg(QString::number(id)) + pictureEndpoint};
        reply_ = manager_->post(requestWithBoundary(endpoint), multiPart_);
        multiPart_->setParent(reply_);

        return reply_;
    }
    catch (const std::runtime_error &err)
    {
        qDebug() << "ProductRepository::add: " << err.what();
        emit errorOccurred(err.what());
    }
    return nullptr;
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
        return;

    for (const auto &picture : pictureList)
    {
        if (!picture.toMap().contains(imageUrlField))
            throw std::runtime_error{"Map does not contains %1."_L1
                                         .arg(imageUrlField)
                                         .toStdString()};

        QHttpPart imagePart;
        imagePart.setHeader(QNetworkRequest::ContentTypeHeader, "image/jpeg"_L1);

        const QUrl imageUrl{picture.toMap()[imageUrlField].toString()};
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

void ProductRepository::validateMap(const QVariantMap &map)
{
    if (!map.contains("id"_L1))
        throw ValidationError{"Map doesn't contains id."};
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
