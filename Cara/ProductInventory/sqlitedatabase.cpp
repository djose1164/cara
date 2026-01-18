#include "sqlitedatabase.h"
#include "productentity.h"
#include "supplier.h"


#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>

#define throw_error(instance, message)                                    \
    do                                                                    \
    {                                                                     \
        qCritical() << "SQLiteProductModel::initDatabase: " << (message); \
        throw std::runtime_error{(instance).lastError().text().toUtf8()}; \
    } while (0)


using namespace Qt::StringLiterals;

namespace
{
constexpr auto RDBMS{"QSQLITE"_L1};
constexpr auto databaseName{"%1/cara_seller_profit.db"_L1};
constexpr auto PRODUCT_OFFLINE_SQL{"create table if not exists product_offline("
                                   "product_id INT PRIMARY KEY,"
                                   "name VARCHAR(64),"
                                   "description VARCHAR(128),"
                                   "category_id INT,"
                                   "price DECIMAL(10, 2),"
                                   "buy_price DECIMAL(10, 2),"
                                   "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                                   "image_url TEXT)"_L1};
constexpr auto SYNC_SQL{"create table if not exists sync(sync_date DATETIME NOT NULL)"_L1};
constexpr auto SELECT_LAST_SYNC{"select sync_date from sync order by sync_date desc"_L1};
constexpr auto INSERT_INTO_SQL{"insert into product_offline(product_id, name, description, category_id, price, buy_price, created_at, image_url)"
                               "values(:product_id, :name, :description, :category_id, :price, :buy_price, :created_at, :image_url)"_L1};
constexpr auto COMMISSION_TABLE{R"(
create table if not exists commission(
    id smallint  primary key,
    unique_code text unique,
    rate decimal(4, 2) not null,
    amount decimal(6, 2) check(amount > 0) not null,
    payment_date datetime,
    admin_id smallint  not null,
    seller_id smallint  not null,
    created_at timestamp not null default CURRENT_TIMESTAMP,
    foreign key (admin_id) references users(id),
    foreign key (seller_id) references salesperson(id),
    constraint chk_rate check(rate > 0)
);)"_L1};
constexpr auto COMMISSION_INDEX{"create index if not exists idx_seller_id on commission(seller_id)"_L1};
constexpr auto COMMISSION_ITEM_TABLE{R"(
create table if not exists commission_item(
    id smallint primary key,
    unique_code text unique,
    commission_id smallint,
    commission_uuid text not null,
    product_id smallint not null,
    quantity smallint  not null,
    unit_commission decimal(6, 2)  not null,
    supplier_id smallint not null,
    foreign key (commission_id)
        references commission(id)
        on delete cascade,
    foreign key (product_id)
        references products(id),
    foreign key (supplier_id)
        references providers(id),
    check(quantity > 0 and unit_commission > 0),
    unique(product_id, commission_id)
);)"_L1};
constexpr auto COMMISSION_ITEM_INDEX{"create index if not exists idx_commissionitem_comission_id on commission_item(commission_id);"_L1};
constexpr auto TRUNCATE_PRODUCT_OFFLINE{"delete from product_offline;"_L1};
constexpr auto TRUNCATE_SELLER{"delete from seller;"_L1};
constexpr auto SELLER_TABLE{
"CREATE TABLE IF NOT EXISTS seller("
"    id integer primary key,"
"    unique_code text unique,"
"    forename text,"
"    surname text"
"    );"_L1
};
constexpr auto SUPPLIER_TABLE{
    "CREATE TABLE IF NOT EXISTS supplier("
    "    id integer primary key,"
    "    unique_code text unique,"
    "    name text"
    ");"_L1
};
constexpr auto INSERT_SUPPLIER{
    "insert into supplier(id, name, unique_code) "
    "values(:id, :name, :unique_code)"_L1};
constexpr auto USER_TABLE{R"(create table if not exists user(
    id integer primary key,
    username text unique,
    forename text not null,
    surname text not null,
    admin_id integer unique not null
))"_L1};
constexpr auto PRODUCT_VIEW{"create view if not exists product as select * from product_offline po ;"_L1};

constexpr auto VW_COMMISSION_DETAIL{R"(
CREATE VIEW IF NOT EXISTS vw_commission_detail
as
    SELECT
        ci.product_id,
        ci.quantity,
        ci.unit_commission,
        ci.supplier_id,
        ci.commission_uuid,
        p.name
    FROM commission_item ci
    JOIN product p
        on p.product_id = ci.product_id;
)"_L1};
}

namespace ProductInventory {

const QString SQLiteDatabase::dbPath_{QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)};

SQLiteDatabase::SQLiteDatabase()
{
}

void SQLiteDatabase::initDatabase()
{
    QSqlDatabase dbInstance{QSqlDatabase::addDatabase(RDBMS)};
    dbInstance.setDatabaseName(databaseName.arg(dbPath_));
    qDebug() << "SQLiteDatabase::initDatabase: " << dbInstance.databaseName();
    qDebug() << "SQLiteDatabase::initDatabase: drivers" << QSqlDatabase::drivers();

    if (!dbInstance.open())
    {
        throw_error(dbInstance, "SQLiteDatabase::initDatabase: Failed to open database.");
    }

    QSqlQuery query;

    // query.exec("drop table product_offline"_L1);
    // query.exec("drop table sync"_L1);
    // query.exec("drop table commission"_L1);
    // query.exec("drop table commission_item"_L1);
    // query.exec("drop table seller"_L1);
    // query.exec("drop table supplier"_L1);
    // query.exec("drop table user"_L1);

    if (!query.exec(PRODUCT_OFFLINE_SQL))
    {
        throw_error(dbInstance, "SQLiteDatabase::initDatabase: Failed to exec PRODUCT_OFFLINE_SQL query.");
    }
    if (!query.exec(PRODUCT_VIEW))
    {
        throw_error(dbInstance, "SQLiteDatabase::initDatabase: Failed to exec PRODUCT_VIEW query.");
    }
    if (!query.exec(VW_COMMISSION_DETAIL))
    {
        throw_error(dbInstance, "SQLiteDatabase::initDatabase: Failed to exec VW_COMMISSION_DETAIL query.");
    }
    if (!query.exec(USER_TABLE))
    {
        throw_error(dbInstance, "SQLiteDatabase::initDatabase: Failed to exec USER_TABLE query.");
    }
    if (!query.exec(SYNC_SQL))
    {
        throw_error(dbInstance, "SQLiteDatabase::initDatabase: Failed to exec SYNC_SQL query.");
    }
    if (!query.exec(SELLER_TABLE))
    {
        throw_error(dbInstance, "SQLiteDatabase::initDatabase: Failed to exec SELLER_TABLE query.");
    }
    if (!query.exec(SUPPLIER_TABLE))
    {
        throw_error(dbInstance, "SQLiteDatabase::initDatabase: Failed to exec SELLER_TABLE query.");
    }
    if (!query.exec(COMMISSION_TABLE)
            or !query.exec(COMMISSION_INDEX)
            or !query.exec(COMMISSION_ITEM_TABLE)
            or !query.exec(COMMISSION_ITEM_INDEX))
    {
        qCritical() << dbInstance.lastError();
        throw_error(dbInstance, "SQLiteDatabase::initDatabase: Failed to COMMISSION_TABLES query.");
    }
}

bool SQLiteDatabase::needToSync()
{
    // Get the latest sync_date from sync table.
    QSqlQuery query{SELECT_LAST_SYNC};
    if (!query.first())
        return true;

    QDate lastSync{ query.value(0).toDate() };
    QDate futureSevenDays{lastSync.addDays(7)};
    qDebug() << "SQLiteDatabase::needToSync: lastSync:" << lastSync;

    return QDate::currentDate() >= futureSevenDays;
}

bool SQLiteDatabase::updateLastSyncDate()
{
    QSqlQuery query;
    query.prepare("insert into sync values(:sync_date)"_L1);
    query.bindValue(":sync_date"_L1, QDateTime::currentDateTime());
    return query.exec();
}

bool SQLiteDatabase::insertProductToDB(const EntityLayer::Product &product)
{
    QSqlQuery query;
    query.prepare(INSERT_INTO_SQL);
    query.bindValue(":product_id"_L1, product.productId());
    query.bindValue(":name"_L1, product.name());
    query.bindValue(":category_id", product.categoryId());
    query.bindValue(":buy_price"_L1, product.buyPrice());
    query.bindValue(":description"_L1, product.description());
    query.bindValue(":price"_L1, product.price());
    query.bindValue(":created_at"_L1, QDateTime::currentDateTime());
    query.bindValue(":image_url"_L1, product.imageUrl());

    const bool res = query.exec();
    if (not res)
        qCritical() << query.lastError().text();
    return res;
}

bool SQLiteDatabase::resetProducts(const QList<EntityLayer::Product> &productList)
{
    QSqlQuery query;
    if (not query.exec(TRUNCATE_PRODUCT_OFFLINE))
    {
        qDebug() << "Could not truncate product_offline. Error: " << query.lastError();
        return false;
    }

    if (not std::ranges::all_of(productList, [](const auto &p) { return insertProductToDB(p); }))
    {
        qWarning() << "SQLiteDatabase::insertProducts: couldn't insert every product!";
        return false;
    }

    return true;
}

bool SQLiteDatabase::insertSupplier(const Supplier &s)
{
    QSqlQuery query;
    query.prepare(INSERT_SUPPLIER);
    query.bindValue(":id", (s.supplierId() == 0 ? QVariant{} : s.supplierId()));
    query.bindValue(":name", s.name());
    query.bindValue(":unique_code", s.uniqueCode());

    const bool res = query.exec();
    if (not res)
        qCritical() << query.lastError().text();
    return res;
}

bool SQLiteDatabase::resetSuppliers(const QList<Supplier> &supplierList)
{
    if (supplierList.isEmpty())
    {
        qDebug() << "SQLiteDatabase::resetSuppliers: supplier list is empty!";
        return false;
    }
    qDebug() << "SQLiteDatabase::resetSuppliers: supplierList.cout: " << supplierList.count();
    QSqlQuery query;
    if (not query.exec(TRUNCATE_SELLER))
    {
        qDebug() << "Could not truncate seller. Error: " << query.lastError();
        return false;
    }

    if (not std::all_of(supplierList.begin(), supplierList.end(), [](const auto &s) {
            return insertSupplier(s);
        }))
    {
        qWarning() << "SQLiteDatabase::resetSuppliers: couldn't insert every supplier! "
                   << query.lastError().text();
    }

    return true;
}

bool SQLiteDatabase::insertSeller(const Entity::Salesperson &seller)
{
    return false;
}

} // namespace ProductInventory
