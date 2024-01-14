#ifndef IINVENTORYREPOSITORY_H
#define IINVENTORYREPOSITORY_H

#include "../entities/productentity.h"
#include "../salesperson/salesperson.h"
#include <QQmlEngine>

namespace Business {
class IInventoryRepository : public QObject
{
    Q_OBJECT
    QML_INTERFACE
public:
    using QObject::QObject;
    using SalespersonId = Entity::Salesperson::SalespersonId;

    virtual ~IInventoryRepository();

    virtual void fetchAll(SalespersonId) = 0;

    virtual void update(const QVariantMap &stocks) = 0;

signals:
    void fetchedOne(const QPointer<EntityLayer::Product> product);
    void fetchedAll(const QList<QPointer<EntityLayer::Product>> &products);
    void errorOccurred(const QString &errorString);
};
}

Q_DECLARE_INTERFACE(Business::IInventoryRepository, "com.cara.IInventoryRepository")
#endif // IINVENTORYREPOSITORY_H
