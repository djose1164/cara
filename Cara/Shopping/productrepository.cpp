#include "productrepository.h"
#include <QFile>
#include <QFileInfo>
#include <QHttpMultiPart>
#include <QJsonArray>
#include <QJsonDocument>
#include "Network/networkrequestsingleton.h"
#include "productpicturerepository.h"
#include "utils.h"
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
    : ProductRepository{new Product::ProductPictureRepository{this}, parent}
{
}

ProductRepository::ProductRepository(Product::PictureRepository *pictureRepository, QObject *parent)
    : QObject{parent}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{nullptr}
    , multiPart_{nullptr}
    , path_{"products"_L1}
    , errorString_{}
    , api_{Network::NetworkRequestSingleton::instance().api()}
    , pictureRepository_{pictureRepository}
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
        pictureRepository_->prepareImagePart(*multiPart_, imagesVariant.toList());
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
    return pictureRepository_->addPicture(id, pictureList);
}

QNetworkReply *ProductRepository::deletePicture(PictureId pictureId)
{
    return pictureRepository_->deletePicture(pictureId);
}

QNetworkReply *ProductRepository::fetchFromSupplier(SupplierId supplierId)
{
    QUrlQuery query;
    query.addQueryItem("supplier_id"_L1, QString::number(supplierId));
    return manager_->get(api_->createRequest(path_, query));
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
    Utils::setBoundaryToRequest(multiPart_->boundary(), request);
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
