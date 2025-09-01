#ifndef ABSTRACTSELLERMODEL_H
#define ABSTRACTSELLERMODEL_H

#include <QVariantMap>


namespace Entity { class Salesperson; }
namespace Seller
{

class AbstractSellerModel
{
public:
    virtual ~AbstractSellerModel() = default;

    virtual void insert(int idx, const Entity::Salesperson &seller) = 0;
    virtual void append(const QVariantMap &seller) = 0;
    virtual int count() const = 0;
    virtual void setSellers(const QList<Entity::Salesperson> &sellerList) = 0;
};

} // namespace Seller

#endif // ABSTRACTSELLERMODEL_H
