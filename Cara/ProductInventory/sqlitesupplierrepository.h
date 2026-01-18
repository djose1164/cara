#ifndef SQLITESUPPLIERREPOSITORY_H
#define SQLITESUPPLIERREPOSITORY_H

#include "abstractsupplierrepository.h"

namespace ProductInventory
{

class SQLiteSupplierRepository : public AbstractSupplierRepository
{
    Q_OBJECT
public:
    explicit SQLiteSupplierRepository(QObject *parent = nullptr);

    void fetchSuppliers() override;
};

} // namespace ProductInventory

#endif // SQLITESUPPLIERREPOSITORY_H
