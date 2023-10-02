#include "inventory.h"

namespace EntityLayer {
Inventory::Inventory(QObject *parent)
    : QObject{parent}
    , inventoryId_{}
    , quantityAvailable_{}
    , minimunStockLevel_{}
    , maximunStockLevel_{}
    , reorderPoint_{}
    , warehouseId_{}
    , product_{}
    , admin_{}
{

}

unsigned int Inventory::inventoryId() const
{
    return inventoryId_;
}

void Inventory::setInventoryId(unsigned int newInventoryId)
{
    if (inventoryId_ == newInventoryId)
        return;
    inventoryId_ = newInventoryId;
    emit inventoryIdChanged();
}

unsigned int Inventory::quantityAvailable() const
{
    return quantityAvailable_;
}

void Inventory::setQuantityAvailable(unsigned int newQuantityAvailable)
{
    if (quantityAvailable_ == newQuantityAvailable)
        return;
    quantityAvailable_ = newQuantityAvailable;
    emit quantityAvailableChanged();
}

unsigned int Inventory::minimunStockLevel() const
{
    return minimunStockLevel_;
}

void Inventory::setMinimunStockLevel(unsigned int newMinimunStockLevel)
{
    if (minimunStockLevel_ == newMinimunStockLevel)
        return;
    minimunStockLevel_ = newMinimunStockLevel;
    emit minimunStockLevelChanged();
}

unsigned int Inventory::maximunStockLevel() const
{
    return maximunStockLevel_;
}

void Inventory::setMaximunStockLevel(unsigned int newMaximunStockLevel)
{
    if (maximunStockLevel_ == newMaximunStockLevel)
        return;
    maximunStockLevel_ = newMaximunStockLevel;
    emit maximunStockLevelChanged();
}

unsigned int Inventory::reorderPoint() const
{
    return reorderPoint_;
}

void Inventory::setReorderPoint(unsigned int newReorderPoint)
{
    if (reorderPoint_ == newReorderPoint)
        return;
    reorderPoint_ = newReorderPoint;
    emit reorderPointChanged();
}

unsigned int Inventory::warehouseId() const
{
    return warehouseId_;
}

void Inventory::setWarehouseId(unsigned int newWarehouseId)
{
    if (warehouseId_ == newWarehouseId)
        return;
    warehouseId_ = newWarehouseId;
    emit warehouseIdChanged();
}

ProductEntity *Inventory::product() const
{
    return product_;
}

void Inventory::setProduct(ProductEntity *newProduct)
{
    if (product_ == newProduct)
        return;
    product_ = newProduct;
    emit productChanged();
}

UserEntity *Inventory::admin() const
{
    return admin_;
}

void Inventory::setAdmin(UserEntity *newAdmin)
{
    if (admin_ == newAdmin)
        return;
    admin_ = newAdmin;
    emit adminChanged();
}
}
