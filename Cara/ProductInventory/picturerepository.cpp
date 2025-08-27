#include "picturerepository.h"

#include <QFile>
#include <QFileInfo>
#include <QHttpPart>

using namespace Qt::Literals::StringLiterals;

namespace
{
constexpr auto imageUrlField{"imageUrl"_L1};
}

namespace ProductInventory
{

PictureRepository::~PictureRepository() = default;

void PictureRepository::prepareImagePart(QHttpMultiPart &multiPart, const QVariantList &pictureList)
{
    qDebug() << "PictureRepository::prepareImagePart: " << pictureList;
    if (pictureList.isEmpty())
        return;

    for (const auto &picture : pictureList)
    {
        if (!picture.toMap().contains(imageUrlField))
            throw std::runtime_error{
                "Map does not contains %1."_L1.arg(imageUrlField).toStdString()};

        QHttpPart imagePart;
        imagePart.setHeader(QNetworkRequest::ContentTypeHeader, "image/jpeg"_L1);

        const QUrl imageUrl{picture.toMap()[imageUrlField].toString()};
#ifdef Q_OS_ANDROID
        const auto img{new QFile(imageUrl.toString())};
#else
        const auto img{new QFile(imageUrl.toLocalFile())};
#endif
        imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                            "form-data; name=\"image\"; filename=\"%1\""_L1.arg(
                                QFileInfo(*img).fileName()));

        if (!img->open(QIODevice::ReadOnly))
        {
            constexpr auto errorString{"Failed to open file for reading"_L1};
            qCritical() << "PictureRepository::prepareImagePart: " << errorString;
            emit errorOccurred(errorString);
            throw std::runtime_error(errorString.toString().toUtf8());
        }

        imagePart.setBodyDevice(img);
        img->setParent(&multiPart);

        multiPart.append(imagePart);
    }
}

}
