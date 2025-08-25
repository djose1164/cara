#include "sqlitedatabase.h"
#include "iproductrepository.h"
#include "productentity.h"
#include "sellerproductrepositoryrest.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>

#define DEFAULT_SELLER_FOR_DB 1

#define throw_error(instance, message)                                         \
    do {                                                                       \
        qCritical() << "SQLiteProductModel::initDatabase: " << (message);      \
        throw std::runtime_error{(instance).lastError().text().toUtf8()};      \
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
    amount decimal(6, 2) check(amount > 0),
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

void saveProductListToDB(const QList<EntityLayer::Product> &productList)
{
    qDebug() << "SQLiteDatabase::saveProductListToDB: inserting products...";
    // For each product in the list, insert it into the database.
    std::ranges::for_each(productList, ProductInventory::SQLiteDatabase::insertProductToDB);

    QSqlQuery query;
    query.prepare("insert into sync values(:sync_date)"_L1);
    query.bindValue(":sync_date"_L1, QDateTime::currentDateTime());
    query.exec();
}

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

    if (!dbInstance.open())
    {
        throw_error(dbInstance, "SQLiteDatabase::initDatabase: Failed to open database.");
    }

    QSqlQuery query;

    // query.exec("drop table product_offline"_L1);
    // query.exec("drop table sync"_L1);
    // query.exec("drop table commission"_L1);
    // query.exec("drop table commission_item"_L1);

    if (!query.exec(PRODUCT_OFFLINE_SQL))
    {
        throw_error(dbInstance, "SQLiteDatabase::initDatabase: Failed to exec query.");
    }
    if (!query.exec(SYNC_SQL))
    {
        throw_error(dbInstance, "SQLiteDatabase::initDatabase: Failed to SYNC_SQL query.");
    }
    if (!query.exec(COMMISSION_TABLE)
            or !query.exec(COMMISSION_INDEX)
            or !query.exec(COMMISSION_ITEM_TABLE)
            or !query.exec(COMMISSION_ITEM_INDEX))
    {
        qCritical() << dbInstance.lastError();
        throw_error(dbInstance, "SQLiteDatabase::initDatabase: Failed to COMMISSION_TABLES query.");
    }

    if (needToSync())
    {
        qDebug() << "SQLiteDatabase::initDatabase: synchronizing database.";
        syncDB();
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

bool SQLiteDatabase::insertProductToDB(const EntityLayer::Product &product)
{
    qDebug() << "SQLiteDatabase::insertProductToDB: inserting productId: " << product.productId();
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

void SQLiteDatabase::syncDB()
{
    // Fetch product list and save it.
    std::unique_ptr<IProductRepository> repository{
        std::make_unique<SellerProductRepositoryREST>(DEFAULT_SELLER_FOR_DB)
    };
    repository->update();

    QObject::connect(repository.get(),
                     &IProductRepository::fetched,
                     [repository = std::move(repository)](
                         const QList<EntityLayer::Product> &products) {
                         QSqlQuery query;
                         if (not query.exec(TRUNCATE_PRODUCT_OFFLINE))
                             qDebug() << "Could not truncate product_offline. Error: " << query.lastError();
                         saveProductListToDB(products);
                     });
}

} // namespace ProductInventory
