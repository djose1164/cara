#ifndef SQLITEPRODUCTREPOSITORY_H
#define SQLITEPRODUCTREPOSITORY_H

#include "iproductrepository.h"

namespace ProductInventory
{

class SQLiteProductRepository : public IProductRepository
{
    Q_OBJECT
public:
    explicit SQLiteProductRepository(QObject *parent = nullptr);

    // IProductRepository interface
    void add(const EntityLayer::Product &product) override;
    void update() override;
    void remove(ProductId productId) override;
};

} // namespace ProductInventory

#endif // SQLITEPRODUCTREPOSITORY_H
