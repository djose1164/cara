#ifndef SQLITEDATABASE_H
#define SQLITEDATABASE_H


#include <QString>

namespace EntityLayer { class Product; }
namespace ProductInventory {

class SQLiteDatabase
{
public:
    SQLiteDatabase();
    static void initDatabase();
    static bool insertProductToDB(const EntityLayer::Product &product);

private:
    static bool needToSync();
    static void syncDB();

    static const QString dbPath_;
};

} // namespace ProductInventory

#endif // SQLITEDATABASE_H
