#ifndef PRODUCTINVENTORYCONTROLLER_H
#define PRODUCTINVENTORYCONTROLLER_H

#include "suppliermodel.h"
#include "productmodel.h"
#include <QObject>
#include <QQmlEngine>

namespace ProductInventory {

class ProductInventoryController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ProductInventory::ProductModel *catalog READ catalog NOTIFY catalogChanged FINAL)
    Q_PROPERTY(ProductInventory::ProductModel *inventory READ inventory NOTIFY inventoryChanged FINAL)
    Q_PROPERTY(ProductInventory::SupplierModel *supplierModel
               READ supplierModel
               NOTIFY supplierModelChanged FINAL)
    QML_ELEMENT
    QML_SINGLETON
public:
    explicit ProductInventoryController(QObject *parent = nullptr);

    ProductModel *catalog() const;
    void setCatalog(ProductModel *newCatalog);

    ProductModel *inventory() const;
    void setInventory(ProductModel *newInventory);

    SupplierModel *supplierModel() const;
    void setSupplierModel(SupplierModel *newSupplierModel);

public slots:
    void updateSellerInventory(int sellerId) noexcept;

signals:
    void catalogChanged();

    void inventoryChanged();

    void supplierModelChanged();

private:
    void initCatalog() noexcept;
    /**
     * @brief updateSupplierModel sets the corresponding model depending on the
     * internet connection. If the device has internet connection, RestSupplierRepository
     * will be used, otherwise, SQLiteSupplierRepository.
     */
    void updateSupplierModel(QNetworkInformation::Reachability reachability);

    ProductModel *catalog_;
    ProductModel *inventory_;
    SupplierModel *supplierModel_;
};

} // namespace ProductInventory

#endif // PRODUCTINVENTORYCONTROLLER_H
