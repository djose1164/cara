#ifndef PICTUREREPOSITORY_H
#define PICTUREREPOSITORY_H

#include <QObject>

class QNetworkReply;
class QHttpMultiPart;
namespace ProductInventory
{

class PictureRepository : public QObject
{
    Q_OBJECT
public:
    using ProductId = int;
    using PictureId = int;

    using QObject::QObject;

    virtual ~PictureRepository();

    [[nodiscard]] virtual QNetworkReply *addPicture(ProductId id, const QVariantList &pictureList) = 0;

    [[nodiscard]] virtual QNetworkReply *deletePicture(PictureId pictureId) = 0;

    void prepareImagePart(QHttpMultiPart &multiPart, const QVariantList &pictureList);

signals:
    void pictureAdded(const QByteArray &data);

    void pictureDeleted();

    void errorOccurred(const QString &errorString);
};

}

#endif // PICTUREREPOSITORY_H
