#include "datapreloader.h"
#include "restsellerrepository.h"
#include "restsupplierrepository.h"
#include "salesperson.h"
#include "sellerproductrepositoryrest.h"
#include "sqlitedatabase.h"
#include "supplier.h"

#include <QSqlQuery>
#include <QSqlError>

using namespace Qt::StringLiterals;

using namespace ProductInventory;
using namespace Seller;

constexpr auto INSERT_SELLER{"insert into seller(id, unique_code, forename, surname) "
                             "values(:id, :unique_code, :forename, :surname)"_L1};

constexpr auto TRUNCATE_SELLER{"delete from seller"_L1};

namespace Billing
{

DataPreloader::DataPreloader(QObject *parent)
    : QObject{parent}
    , supplierRepo_{new RestSupplierRepository(this)}
    , productRepo_{new SellerProductRepositoryREST(this)}
    , sellerRepo_{}
{
    connect(supplierRepo_,
            &AbstractSupplierRepository::suppliersFetched,
            this,
            &DataPreloader::suppliersFetched);
    connect(productRepo_, &IProductRepository::fetched, this, &DataPreloader::productsFetched);
}

void DataPreloader::preload()
{
    if (not SQLiteDatabase::needToSync())
        return;

    supplierRepo_->fetchSuppliers();
    productRepo_->update();
    sellerRepo_->fetchSellers();
}

void DataPreloader::suppliersFetched(const QList<ProductInventory::Supplier> &supplierList)
{
    qDebug() << "DataPreloader::suppliersFetched: " << QVariant::fromValue(supplierList);
    if (not SQLiteDatabase::resetSuppliers(supplierList))
        emit errorOccurred("Couldn't update suppliers when syncing database!"_L1);
}

void DataPreloader::sellersFetched(const QList<Entity::Salesperson> &sellerList)
{
    qDebug() << "DataPreloader::sellersFetched: inserting sellers...";
    QSqlQuery query;
    if (sellerList.isEmpty() or not query.exec(TRUNCATE_SELLER))
    {
        qDebug() << "DataPreloader::sellersFetched: seller list is empty or SQL error! "
                 << query.lastError().text();
        return;
    }

    bool res{true};
    std::ranges::for_each(sellerList, [&res](const auto &seller) {
        qDebug() << "DataPreloader::sellersFetched: seller: " << seller.forename();
        res = res and insertSeller(seller);
    });
}

bool DataPreloader::insertSeller(const Entity::Salesperson &seller)
{
    QSqlQuery query;
    query.prepare(INSERT_SELLER);
    query.bindValue(":unique_code"_L1, QVariant{});
    query.bindValue("id"_L1, seller.salespersonId());
    query.bindValue(":forename"_L1, seller.forename());
    query.bindValue(":surname"_L1, seller.surname());

    const bool res{query.exec()};
    if (not res)
        qWarning() << "DataPreloader::insertSeller: " << query.lastError().text();
    return res;
}

DataPreloader::AdminId DataPreloader::adminId() const
{
    return adminId_;
}

void DataPreloader::setAdminId(AdminId newAdminId)
{
    if (adminId_ == newAdminId)
        return;
    adminId_ = newAdminId;
    emit adminIdChanged();
}
void DataPreloader::productsFetched(const QList<EntityLayer::Product> &productList)
{
    if (ProductInventory::SQLiteDatabase::resetProducts(productList))
        ;
    else
        emit errorOccurred("Couldn't sync database!"_L1);
}

void Billing::DataPreloader::classBegin()
{
}

void Billing::DataPreloader::componentComplete()
{
    qDebug() << "Billing::DataPreloader::componentComplete: " << adminId_;
    const auto restSellerRepo{new RestSellerRepository(this)};
    restSellerRepo->setAdminId(adminId_);
    sellerRepo_ = restSellerRepo;

    connect(sellerRepo_, &AbstractSellerRepository::fetched, this, &DataPreloader::sellersFetched);
}

} // namespace Billing
