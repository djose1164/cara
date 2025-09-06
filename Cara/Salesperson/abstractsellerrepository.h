#ifndef ABSTRACTSELLERREPOSITORY_H
#define ABSTRACTSELLERREPOSITORY_H

#include <QObject>

namespace Entity
{
class Salesperson;
}
namespace Seller
{

class AbstractSellerRepository : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    virtual ~AbstractSellerRepository() = default;

    virtual void fetchSellers() = 0;

signals:
    void fetched(const QList<Entity::Salesperson> &sellerList);
};

} // namespace Seller

#endif // ABSTRACTSELLERREPOSITORY_H
