#ifndef PRODUCTPICTUREREPOSITORY_H
#define PRODUCTPICTUREREPOSITORY_H

#include "picturerepository.h"

#include <QNetworkRequest>

class QHttpMultiPart;
class QNetworkRequestFactory;
class QRestAccessManager;
namespace ProductInventory
{

class ProductPictureRepository : public PictureRepository
{
    Q_OBJECT
public:
    explicit ProductPictureRepository(QObject *parent = nullptr);

    // PictureRepository interface
    QNetworkReply *addPicture(ProductId id, const QVariantList &pictureList) override;
    QNetworkReply *deletePicture(PictureId pictureId) override;

    QString errorString() const;
    void setErrorString(const QString &newErrorString);

private:
    QNetworkRequest requestWithBoundary(QAnyStringView endpoint = {}) const;

    QRestAccessManager *manager_;
    QNetworkReply *reply_;
    QHttpMultiPart *multiPart_;
    std::shared_ptr<QNetworkRequestFactory> api_;
    const QString path_;
    QString errorString_;
};

} // namespace ProductInventory

#endif // PRODUCTPICTUREREPOSITORY_H
