#ifndef IINVENTORYREPOSITORY_H
#define IINVENTORYREPOSITORY_H

#include <QVariantMap>

namespace Inventory {
class IInventoryRepository
{
public:
    virtual ~IInventoryRepository();

    virtual void fetchAll() = 0;

    virtual void update(const QVariantMap &stocks) = 0;
};
}

#endif // IINVENTORYREPOSITORY_H
