#ifndef IPRODUCTREPOSITORY_H
#define IPRODUCTREPOSITORY_H

#include <QVariantMap>

namespace Catalogue {
class IProductRepository
{
public:
    virtual ~IProductRepository();

    virtual void add(const QVariantMap &product) = 0;
    
    virtual void update(const QVariantMap &product) = 0;

    virtual void fetchAll() = 0;
};
}

#endif // IPRODUCTREPOSITORY_H
