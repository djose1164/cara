#ifndef INVENTORY_H
#define INVENTORY_H

#include <QObject>
#include <QQmlEngine>

#include "productentity.h"

namespace EntityLayer {
class Inventory : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int inventoryId READ inventoryId WRITE setInventoryId NOTIFY inventoryIdChanged FINAL)
    Q_PROPERTY(int quantityAvailable READ quantityAvailable WRITE setQuantityAvailable NOTIFY quantityAvailableChanged FINAL)
    Q_PROPERTY(int minimunStockLevel READ minimunStockLevel WRITE setMinimunStockLevel NOTIFY minimunStockLevelChanged FINAL)
    Q_PROPERTY(int maximunStockLevel READ maximunStockLevel WRITE setMaximunStockLevel NOTIFY maximunStockLevelChanged FINAL)
    Q_PROPERTY(int reorderPoint READ reorderPoint WRITE setReorderPoint NOTIFY reorderPointChanged FINAL)
    Q_PROPERTY(int warehouseId READ warehouseId WRITE setWarehouseId NOTIFY warehouseIdChanged FINAL)
    Q_PROPERTY(AdminId *adminId READ adminId WRITE setAdminId NOTIFY adminIdChanged FINAL)
public:
    using AdminId = int;

    explicit Inventory(QObject *parent = nullptr);

    int inventoryId() const;
    void setInventoryId(int newInventoryId);

    int quantityAvailable() const;
    void setQuantityAvailable(int newQuantityAvailable);

    int minimunStockLevel() const;
    void setMinimunStockLevel(int newMinimunStockLevel);

    int maximunStockLevel() const;
    void setMaximunStockLevel(int newMaximunStockLevel);

    int reorderPoint() const;
    void setReorderPoint(int newReorderPoint);

    int warehouseId() const;
    void setWarehouseId(int newWarehouseId);
    
    Product *product() const;
    void setProduct(Product *newProduct);

    AdminId *adminId() const;
    void setAdminId(AdminId *newAdminId);

signals:
    void inventoryIdChanged();

    void quantityAvailableChanged();

    void minimunStockLevelChanged();

    void maximunStockLevelChanged();

    void reorderPointChanged();

    void warehouseIdChanged();

    void productChanged();

    void adminChanged();

    void adminIdChanged();

private:
    unsigned inventoryId_;
    unsigned quantityAvailable_;
    unsigned minimunStockLevel_;
    unsigned maximunStockLevel_;
    unsigned reorderPoint_;
    unsigned warehouseId_;
    
    Product *product_;
    AdminId *adminId_;
};
}

#endif // INVENTORY_H
