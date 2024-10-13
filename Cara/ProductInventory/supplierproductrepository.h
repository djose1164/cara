#ifndef SUPPLIERPRODUCTREPOSITORY_H
#define SUPPLIERPRODUCTREPOSITORY_H

#include "iproductrepository.h"
#include <QQmlEngine>

namespace ProductInventory {

class SupplierProductRepository : public ProductInventory::IProductRepository
{
    Q_OBJECT
    Q_PROPERTY(int supplierId READ supplierId WRITE setSupplierId NOTIFY supplierIdChanged FINAL)
    QML_ELEMENT
public:
    using SupplierId = int;

    explicit SupplierProductRepository(QObject *parent = nullptr);

    // IProductRepository interface
    void add(const EntityLayer::Product &product) override;
    void update() override;
    void remove(SupplierId supplierId) override;

    SupplierId supplierId() const;
    void setSupplierId(SupplierId newSupplierId);

signals:
    void supplierIdChanged();


private:
    SupplierId supplierId_;
};

} // namespace ProductInventory

#endif // SUPPLIERPRODUCTREPOSITORY_H
