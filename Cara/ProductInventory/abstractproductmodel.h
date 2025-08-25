#ifndef ABSTRACTPRODUCTMODEL_H
#define ABSTRACTPRODUCTMODEL_H

#include "productentity.h"


namespace ProductInventory
{

class AbstractProductModel
{

public:
    virtual ~AbstractProductModel() = default;

    virtual EntityLayer::Product get(int index) const = 0;
    virtual bool contains(const EntityLayer::Product &product) const = 0;
    virtual QModelIndex find(int productId) const = 0;
    virtual QJsonArray toJsonArray() const = 0;
};

} // ProductInventory

#endif // ABSTRACTPRODUCTMODEL_H
