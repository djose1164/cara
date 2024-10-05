#ifndef INVENTORYPRODUCTREPOSITORY_H
#define INVENTORYPRODUCTREPOSITORY_H

#include "iproductrepository.h"
#include <QQmlEngine>

namespace ProductInventory
{

class CatalogProductRepository : public IProductRepository
{
    Q_OBJECT
    QML_ELEMENT
public:

    explicit CatalogProductRepository(QObject *parent = nullptr);

public slots:
    void add(const EntityLayer::Product &product) override;
    void remove(ProductId productId) override;

signals:

private:
    void update() override;


};

} // namespace ProductInventory

#endif // INVENTORYPRODUCTREPOSITORY_H
