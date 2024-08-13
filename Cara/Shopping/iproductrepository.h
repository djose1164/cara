#ifndef IPRODUCTREPOSITORY_H
#define IPRODUCTREPOSITORY_H

#include <QVariantMap>
class QNetworkReply;

namespace Catalogue {
class IProductRepository
{
public:
    using ProductId = int;

    virtual ~IProductRepository();

    virtual void add(const QVariantMap &product) = 0;

    [[nodiscard]] virtual QNetworkReply *read(ProductId id) = 0;
    
    virtual void update(const QVariantMap &product) = 0;

    virtual void fetchAll() = 0;
};
}

#endif // IPRODUCTREPOSITORY_H
