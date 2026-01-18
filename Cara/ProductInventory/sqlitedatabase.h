#ifndef SQLITEDATABASE_H
#define SQLITEDATABASE_H

#include <QString>

namespace Entity { class Salesperson; }
namespace EntityLayer { class Product; }
namespace ProductInventory {

class Supplier;
class SQLiteDatabase
{
public:
    SQLiteDatabase();
    static void initDatabase();

    static bool insertProductToDB(const EntityLayer::Product &product);
    static bool resetProducts(const QList<EntityLayer::Product> &productList);

    static bool insertSupplier(const Supplier &s);
    static bool resetSuppliers(const QList<Supplier> &supplierList);

    static bool insertSeller(const Entity::Salesperson &seller);
    static bool resetSellers(const QList<Entity::Salesperson> &sellerList);

    static bool needToSync();
    static bool updateLastSyncDate();

private:
    static const QString dbPath_;
};

} // namespace ProductInventory

#endif // SQLITEDATABASE_H
