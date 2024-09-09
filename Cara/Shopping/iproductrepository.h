#ifndef IPRODUCTREPOSITORY_H
#define IPRODUCTREPOSITORY_H

#include <QVariantMap>
class QNetworkReply;

namespace Catalogue {
class IProductRepository
{
public:
    using ProductId = int;
    using SupplierId = int;

    virtual ~IProductRepository();

    virtual void add(const QVariantMap &product) = 0;

    [[nodiscard]] virtual QNetworkReply *read(ProductId id) = 0;
    
    virtual void update(const QVariantMap &product) = 0;

    virtual void fetchAll() = 0;

    [[nodiscard]] virtual QNetworkReply *fetchFromSupplier(SupplierId supplierId) = 0;
};
}

#endif // IPRODUCTREPOSITORY_H
