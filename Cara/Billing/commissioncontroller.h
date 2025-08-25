#ifndef COMMISSIONCONTROLLER_H
#define COMMISSIONCONTROLLER_H

#include "ProductInventory/productmodel.h"
#include <QAbstractItemModel>
#include <QNetworkInformation>
#include <QQmlEngine>

namespace Seller { class AbstractSellerModel; }

namespace Billing
{
class AbstractCommissionModel;
class CommissionController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ProductInventory::ProductModel *productModel READ productModel WRITE setProductModel
                   NOTIFY productModelChanged FINAL)
    Q_PROPERTY(QAbstractItemModel *commissionModel READ commissionModel NOTIFY commissionModelChanged FINAL)
    Q_PROPERTY(ProductInventory::ProductModel *selectedProducts READ selectedProducts WRITE
                   setSelectedProducts NOTIFY selectedProductsChanged FINAL)
    Q_PROPERTY(int supplierId READ supplierId WRITE setSellerId NOTIFY supplierIdChanged FINAL)
    QML_ELEMENT
public:
    using SellerId = int;

    explicit CommissionController(QObject *parent = nullptr);

    QAbstractItemModel *commissionModel() const;

    ProductInventory::ProductModel *selectedProducts() const;
    void setSelectedProducts(ProductInventory::ProductModel *newSelectedProducts);

    int supplierId() const;
    void setSellerId(int newSellerId);

    ProductInventory::ProductModel *productModel() const;
    void setProductModel(ProductInventory::ProductModel *newProductModel);

public slots:
    void append(const QVariantMap &map);
    void fetchCommissions();
    void fetchCommissionsBySellerId(SellerId sellerId);

private slots:
    void networkStatusChanged(QNetworkInformation::Reachability reachability);

signals:
    void selectedProductsChanged();

    void supplierIdChanged();

    void productModelChanged();

    void commissionModelChanged();

protected:
    void setCommissionModel(AbstractCommissionModel *newCommissionModel);

private:
    int supplierId_;
    AbstractCommissionModel *commissionModel_;
    ProductInventory::ProductModel *productModel_;
    ProductInventory::ProductModel *selectedProducts_;
    Seller::AbstractSellerModel *sellerModel_;
};
}

#endif // COMMISSIONCONTROLLER_H
