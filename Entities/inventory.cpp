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
    , adminId_{}
{

}

int Inventory::inventoryId() const
{
    return inventoryId_;
}

void Inventory::setInventoryId(int newInventoryId)
{
    if (inventoryId_ == newInventoryId)
        return;
    inventoryId_ = newInventoryId;
    emit inventoryIdChanged();
}

int Inventory::quantityAvailable() const
{
    return quantityAvailable_;
}

void Inventory::setQuantityAvailable(int newQuantityAvailable)
{
    if (quantityAvailable_ == newQuantityAvailable)
        return;
    quantityAvailable_ = newQuantityAvailable;
    emit quantityAvailableChanged();
}

int Inventory::minimunStockLevel() const
{
    return minimunStockLevel_;
}

void Inventory::setMinimunStockLevel(int newMinimunStockLevel)
{
    if (minimunStockLevel_ == newMinimunStockLevel)
        return;
    minimunStockLevel_ = newMinimunStockLevel;
    emit minimunStockLevelChanged();
}

int Inventory::maximunStockLevel() const
{
    return maximunStockLevel_;
}

void Inventory::setMaximunStockLevel(int newMaximunStockLevel)
{
    if (maximunStockLevel_ == newMaximunStockLevel)
        return;
    maximunStockLevel_ = newMaximunStockLevel;
    emit maximunStockLevelChanged();
}

int Inventory::reorderPoint() const
{
    return reorderPoint_;
}

void Inventory::setReorderPoint(int newReorderPoint)
{
    if (reorderPoint_ == newReorderPoint)
        return;
    reorderPoint_ = newReorderPoint;
    emit reorderPointChanged();
}

int Inventory::warehouseId() const
{
    return warehouseId_;
}

void Inventory::setWarehouseId(int newWarehouseId)
{
    if (warehouseId_ == newWarehouseId)
        return;
    warehouseId_ = newWarehouseId;
    emit warehouseIdChanged();
}

Product *Inventory::product() const
{
    return product_;
}

void Inventory::setProduct(Product *newProduct)
{
    if (product_ == newProduct)
        return;
    product_ = newProduct;
    emit productChanged();
}

Inventory::AdminId *Inventory::adminId() const
{
    return adminId_;
}

void Inventory::setAdminId(AdminId *newAdminId)
{
    if (adminId_ == newAdminId)
        return;
    adminId_ = newAdminId;
    emit adminIdChanged();
}
}
