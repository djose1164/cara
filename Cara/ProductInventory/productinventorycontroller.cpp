#include "productinventorycontroller.h"
#include "catalogproductrepository.h"
#include "sellerproductrepositoryrest.h"

namespace ProductInventory {

ProductInventoryController::ProductInventoryController(QObject *parent)
    : QObject{parent}
    , catalog_{new ProductModel{this}}
    , inventory_{new ProductModel{this}}
{
    initCatalog();
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

} // namespace ProductInventory
