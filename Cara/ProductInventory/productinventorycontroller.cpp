#include "productinventorycontroller.h"
#include "abstractsupplierrepository.h"
#include "catalogproductrepository.h"
#include "restsupplierrepository.h"
#include "sellerproductrepositoryrest.h"
#include "sqlitesupplierrepository.h"
#include <QNetworkInformation>

namespace ProductInventory {

ProductInventoryController::ProductInventoryController(QObject *parent)
    : QObject{parent}
    , catalog_{new ProductModel{this}}
    , inventory_{new ProductModel{this}}
    , supplierModel_{new SupplierModel(this)}
{
    initCatalog();
    updateSupplierModel(QNetworkInformation::instance()->reachability());
    connect(QNetworkInformation::instance(),
            &QNetworkInformation::reachabilityChanged,
            this,
            &ProductInventoryController::updateSupplierModel);
}

ProductModel *ProductInventoryController::catalog() const
{
    return catalog_;
}

void ProductInventoryController::setCatalog(ProductModel *newCatalog)
{
    if (catalog_ == newCatalog)
        return;
    catalog_ = newCatalog;
    emit catalogChanged();
}

ProductModel *ProductInventoryController::inventory() const
{
    return inventory_;
}

void ProductInventoryController::setInventory(ProductModel *newInventory)
{
    if (inventory_ == newInventory)
        return;
    inventory_ = newInventory;
    emit inventoryChanged();
}

void ProductInventoryController::updateSellerInventory(int sellerId) noexcept
{
    const auto tempRepo{new SellerProductRepositoryREST};
    tempRepo->setSellerId(sellerId);

    inventory_->setRepository(tempRepo);
    inventory_->update();

    connect(tempRepo, &IProductRepository::fetched, this, [this, tempRepo = std::move(tempRepo)] {
        tempRepo->deleteLater();
        inventory_->setRepository(nullptr);
        qDebug() << "ProductInventoryController::updateSellerInventory: tempRepo->deleteLater();";
    });
}

void ProductInventoryController::initCatalog() noexcept
{
    const auto tempRepo{new CatalogProductRepository};
    catalog_->setRepository(tempRepo);
    catalog_->update();

    connect(tempRepo, &IProductRepository::fetched, this, [this, tempRepo = std::move(tempRepo)] {
        tempRepo->deleteLater();
        inventory_->setRepository(nullptr);
    });
}

void ProductInventoryController::updateSupplierModel(QNetworkInformation::Reachability reachability)
{
    if (supplierModel_->repository())
        supplierModel_->repository()->deleteLater();


    if (AbstractSupplierRepository *repository;
        reachability == QNetworkInformation::Reachability::Online)
    {
        repository = new RestSupplierRepository(supplierModel_);
        supplierModel_->setRepository(repository);
    }
    else
    {
        repository = new SQLiteSupplierRepository(supplierModel_);
        supplierModel_->setRepository(repository);
    }

    qDebug() << "ProductInventoryController::updateSupplierModel: fetchig suppliers...";
    supplierModel_->fetchSuppliers();
}

SupplierModel *ProductInventoryController::supplierModel() const
{
    return supplierModel_;
}

void ProductInventoryController::setSupplierModel(SupplierModel *newSupplierModel)
{
    if (supplierModel_ == newSupplierModel)
        return;
    supplierModel_ = newSupplierModel;
    emit supplierModelChanged();
}

} // namespace ProductInventory
