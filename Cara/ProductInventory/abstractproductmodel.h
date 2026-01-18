#ifndef ABSTRACTPRODUCTMODEL_H
#define ABSTRACTPRODUCTMODEL_H

#include "productentity.h"


namespace ProductInventory
{

class AbstractProductModel
{

public:
    using ProductId = int;
    virtual ~AbstractProductModel() = default;

    virtual EntityLayer::Product get(int index) const = 0;
    virtual bool contains(const EntityLayer::Product &product) const = 0;
    virtual QModelIndex find(int productId) const = 0;
    virtual QJsonArray toJsonArray() const = 0;
    virtual EntityLayer::Product getProductById(ProductId productId) const
    {
        Q_UNUSED(productId);
        return {};
    };
};

} // ProductInventory

#endif // ABSTRACTPRODUCTMODEL_H
