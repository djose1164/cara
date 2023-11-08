#ifndef INVENTORY_H
#define INVENTORY_H

#include <QObject>
#include <QQmlEngine>

#include "productentity.h"
#include "userentity.h"

namespace EntityLayer {
class Inventory : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(unsigned int inventoryId READ inventoryId WRITE setInventoryId NOTIFY inventoryIdChanged FINAL)
    Q_PROPERTY(unsigned int quantityAvailable READ quantityAvailable WRITE setQuantityAvailable NOTIFY quantityAvailableChanged FINAL)
    Q_PROPERTY(unsigned int minimunStockLevel READ minimunStockLevel WRITE setMinimunStockLevel NOTIFY minimunStockLevelChanged FINAL)
    Q_PROPERTY(unsigned int maximunStockLevel READ maximunStockLevel WRITE setMaximunStockLevel NOTIFY maximunStockLevelChanged FINAL)
    Q_PROPERTY(unsigned int reorderPoint READ reorderPoint WRITE setReorderPoint NOTIFY reorderPointChanged FINAL)
    Q_PROPERTY(unsigned int warehouseId READ warehouseId WRITE setWarehouseId NOTIFY warehouseIdChanged FINAL)

public:
    explicit Inventory(QObject *parent = nullptr);

    unsigned int inventoryId() const;
    void setInventoryId(unsigned int newInventoryId);

    unsigned int quantityAvailable() const;
    void setQuantityAvailable(unsigned int newQuantityAvailable);

    unsigned int minimunStockLevel() const;
    void setMinimunStockLevel(unsigned int newMinimunStockLevel);

    unsigned int maximunStockLevel() const;
    void setMaximunStockLevel(unsigned int newMaximunStockLevel);

    unsigned int reorderPoint() const;
    void setReorderPoint(unsigned int newReorderPoint);

    unsigned int warehouseId() const;
    void setWarehouseId(unsigned int newWarehouseId);
    
    Product *product() const;
    void setProduct(Product *newProduct);
    
    Entity::UserEntity *admin() const;
    void setAdmin(Entity::UserEntity *newAdmin);

signals:
    void inventoryIdChanged();

    void quantityAvailableChanged();

    void minimunStockLevelChanged();

    void maximunStockLevelChanged();

    void reorderPointChanged();

    void warehouseIdChanged();

    void productChanged();

    void adminChanged();

private:
    unsigned inventoryId_;
    unsigned quantityAvailable_;
    unsigned minimunStockLevel_;
    unsigned maximunStockLevel_;
    unsigned reorderPoint_;
    unsigned warehouseId_;
    
    Product *product_;
    Entity::UserEntity *admin_;
};
}

#endif // INVENTORY_H
