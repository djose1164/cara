#ifndef SUPPLIERPRODUCTREPOSITORY_H
#define SUPPLIERPRODUCTREPOSITORY_H

#include "iproductrepository.h"
#include <QQmlEngine>

namespace ProductInventory {

class SupplierProductRepository : public ProductInventory::IProductRepository
{
    Q_OBJECT
    Q_PROPERTY(int productId READ productId WRITE setProductId NOTIFY productIdChanged FINAL)
    QML_ELEMENT
public:
    explicit SupplierProductRepository(QObject *parent = nullptr);

    // IProductRepository interface
    void add(const EntityLayer::Product &product) override;
    void update() override;
    void remove(ProductId productId) override;

    ProductId productId() const;
    void setProductId(ProductId newProductId);

signals:
    void productIdChanged();


private:
    ProductId productId_;
};

} // namespace ProductInventory

#endif // SUPPLIERPRODUCTREPOSITORY_H
