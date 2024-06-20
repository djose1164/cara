#ifndef IINVENTORYREPOSITORY_H
#define IINVENTORYREPOSITORY_H

#include "productentity.h"
#include <QQmlEngine>

namespace Business {
class IInventoryRepository : public QObject
{
    Q_OBJECT
    QML_INTERFACE
public:
    using QObject::QObject;
    using SalespersonId = int;
    using ProductId = int;

    virtual ~IInventoryRepository();

    virtual void fetchAll(SalespersonId) = 0;

    virtual void fetchAnyByProductId(ProductId productId) = 0;

    virtual void update(const QVariantMap &stocks) = 0;

signals:
    void fetchedOne(const EntityLayer::Product &product);
    void fetchedAll(const QList<EntityLayer::Product> &products);
    void errorOccurred(const QString &errorString);
    void anyFetched(const QList<EntityLayer::Product> &products);
};
}

Q_DECLARE_INTERFACE(Business::IInventoryRepository, "com.cara.IInventoryRepository")
#endif // IINVENTORYREPOSITORY_H
