#ifndef IPRODUCTREPOSITORY_H
#define IPRODUCTREPOSITORY_H

#include <QVariantMap>
class QNetworkReply;

namespace Catalogue {
class IProductRepository
{
public:
    using ProductId = int;
    using PictureId = int;

    virtual ~IProductRepository();

    virtual void add(const QVariantMap &product) = 0;

    [[nodiscard]] virtual QNetworkReply *read(ProductId id) = 0;
    
    virtual void update(const QVariantMap &product) = 0;

    virtual void fetchAll() = 0;

    [[nodiscard]] virtual QNetworkReply *addPicture(ProductId id, const QVariantList &pictureList) = 0;

    [[nodiscard]] virtual QNetworkReply *deletePicture(PictureId pictureId) = 0;
};
}

#endif // IPRODUCTREPOSITORY_H
