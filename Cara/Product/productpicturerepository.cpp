#include "productpicturerepository.h"
#include "Network/networkrequestsingleton.h"
#include "Network/qnamsingleton.h"
#include "utils.h"

#include <QFile>
#include <QFileInfo>
#include <QHttpMultiPart>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QRestAccessManager>

using namespace Qt::Literals::StringLiterals;

namespace
{
constexpr auto resourceID{"/%1"_L1};
constexpr auto pictureEndpoint{"/picture"_L1};
constexpr auto imageUrlField{"imageUrl"_L1};
}

namespace Product
{

ProductPictureRepository::ProductPictureRepository(QObject *parent)
    : PictureRepository{parent}
    , manager_{new QRestAccessManager{QNAMSingleton::instance().qnam(), this}}
    , reply_{}
    , multiPart_{}
    , api_{Network::NetworkRequestSingleton::instance().api()}
    , path_{"products"_L1}
    , errorString_{}
{
}

QNetworkReply *ProductPictureRepository::addPicture(ProductId id, const QVariantList &pictureList)
{
    try
    {
        multiPart_ = new QHttpMultiPart(QHttpMultiPart::FormDataType, this);
        prepareImagePart(*multiPart_, pictureList);

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

QNetworkReply *ProductPictureRepository::deletePicture(PictureId pictureId)
{
    return manager_->deleteResource(
        api_->createRequest(path_ + pictureEndpoint + resourceID.arg(QString::number(pictureId))));
}

QString ProductPictureRepository::errorString() const
{
    return errorString_;
}

void ProductPictureRepository::setErrorString(const QString &newErrorString)
{
    errorString_ = newErrorString;
}

QNetworkRequest ProductPictureRepository::requestWithBoundary(QAnyStringView endpoint) const
{
    QNetworkRequest request{api_->createRequest(path_ + endpoint.toString())};
    Utils::setBoundaryToRequest(multiPart_->boundary(), request);
    return request;
}

} // namespace Product
