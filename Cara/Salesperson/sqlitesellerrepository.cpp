#include "sqlitesellerrepository.h"

#include <QSqlQuery>

namespace Seller
{

SQLiteSellerRepository::SQLiteSellerRepository(QObject *parent)
    : AbstractSellerRepository{parent}
{
}

void SQLiteSellerRepository::fetchSellers()
{
    QSqlQuery query;
    query.exec("");
}

} // namespace Seller
