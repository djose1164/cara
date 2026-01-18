#include "sqlitesupplierrepository.h"
#include <QSqlQuery>
#include <QSqlError>

using namespace Qt::StringLiterals;

constexpr auto SELECT_ALL{"select id, name, unique_code from supplier"_L1};

namespace ProductInventory
{

SQLiteSupplierRepository::SQLiteSupplierRepository(QObject *parent)
    : AbstractSupplierRepository{parent}
{
}

void SQLiteSupplierRepository::fetchSuppliers()
{
    QSqlQuery query;
    if (not query.exec(SELECT_ALL))
    {
        const QString errmsg{query.lastError().text()};
        qWarning() << "SQLiteSupplierRepository::fetchSuppliers: " << errmsg;
        emit errorOccurred(errmsg);
        return;
    }

    QList<Supplier> suppliers;
    while (query.next())
    {
        Supplier s;
        s.setSupplierId(query.value(0).toInt());
        s.setName(query.value(1).toString());
        s.setUniqueCode(query.value(2).toUuid());
        suppliers.append(std::move(s));
    }

    emit suppliersFetched(suppliers);
}

} // namespace ProductInventory
