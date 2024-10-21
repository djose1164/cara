#ifndef PRODUCTINVENTORYCONTROLLER_H
#define PRODUCTINVENTORYCONTROLLER_H

#include "iproductrepository.h"
#include "productmodel.h"
#include <QObject>
#include <QQmlEngine>

namespace ProductInventory {

class ProductInventoryController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ProductModel *catalog READ catalog NOTIFY catalogChanged FINAL)
    Q_PROPERTY(ProductModel *inventory READ inventory NOTIFY inventoryChanged FINAL)
    Q_PROPERTY(IProductRepository *repository READ repository WRITE setRepository NOTIFY repositoryChanged FINAL)
    QML_ELEMENT
    QML_SINGLETON
public:
    explicit ProductInventoryController(QObject *parent = nullptr);

    ProductModel *catalog() const;
    void setCatalog(ProductModel *newCatalog);

    ProductModel *inventory() const;
    void setInventory(ProductModel *newInventory);

    IProductRepository *repository() const;
    void setRepository(IProductRepository *newRepository);

public slots:
    void updateSellerInventory(int sellerId) noexcept;

signals:
    void catalogChanged();

    void inventoryChanged();

    void repositoryChanged();

private:
    void initCatalog() noexcept;

    ProductModel *catalog_;
    ProductModel *inventory_;
    IProductRepository *repository_;
};

} // namespace ProductInventory

#endif // PRODUCTINVENTORYCONTROLLER_H
