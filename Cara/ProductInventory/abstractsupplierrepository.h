#ifndef ABSTRACTSUPPLIERREPOSITORY_H
#define ABSTRACTSUPPLIERREPOSITORY_H

#include "supplier.h"

#include <QQmlEngine>

namespace ProductInventory
{

class AbstractSupplierRepository : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS
public:
    using QObject::QObject;

    virtual ~AbstractSupplierRepository() = default;
    virtual void fetchSuppliers() = 0;

signals:
    void suppliersFetched(const QList<ProductInventory::Supplier> &suppliers);
    void errorOccurred(const QString &errorString);

};

} // namespace ProductInventory

#endif // ABSTRACTSUPPLIERREPOSITORY_H
