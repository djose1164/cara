#include "productinventorycontroller.h"
#include "catalogproductrepository.h"

namespace ProductInventory {

ProductInventoryController::ProductInventoryController(QObject *parent)
    : QObject{parent}
    , catalog_{new ProductModel{this}}
    , inventory_{new ProductModel{this}}
{
    const auto tempRepo{new CatalogProductRepository};
    connect(tempRepo, &IProductRepository::fetched,
            this, [tempRepo]{ tempRepo->deleteLater(); });

    catalog_->setRepository(tempRepo);
    catalog_->update();
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

IProductRepository *ProductInventoryController::repository() const
{
    return repository_;
}

void ProductInventoryController::setRepository(IProductRepository *newRepository)
{
    if (repository_ == newRepository)
        return;
    repository_ = newRepository;
    emit repositoryChanged();
}

} // namespace ProductInventory
