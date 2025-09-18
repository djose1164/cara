#ifndef COMMISSIONCONTROLLER_H
#define COMMISSIONCONTROLLER_H

#include "ProductInventory/productmodel.h"
#include "abstractcommissionmodel.h"
#include <QAbstractItemModel>
#include <QNetworkInformation>
#include <QQmlEngine>

namespace Billing
{

class CommissionController : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_PROPERTY(ProductInventory::ProductModel *productModel
               READ productModel
               WRITE setProductModel
               NOTIFY productModelChanged FINAL)
    Q_PROPERTY(QAbstractItemModel *commissionModel
               READ commissionModel
               NOTIFY commissionModelChanged FINAL)
    Q_PROPERTY(ProductInventory::ProductModel *selectedProducts
               READ selectedProducts
               WRITE setSelectedProducts
               NOTIFY selectedProductsChanged FINAL)
    Q_PROPERTY(int supplierId
               READ supplierId
               WRITE setSupplierId
               NOTIFY supplierIdChanged FINAL)
    Q_PROPERTY(QAbstractItemModel *sellerModel
               READ sellerModel
               WRITE setSellerModel
               NOTIFY sellerModelChanged FINAL)
    Q_PROPERTY(int adminId
                READ adminId
                WRITE setAdminId
                NOTIFY adminIdChanged FINAL REQUIRED)
    Q_PROPERTY(bool busy READ busy WRITE setBusy NOTIFY busyChanged FINAL)
    QML_ELEMENT
public:
    enum class Error
    {
        ProductModelError,
        CommissionModelError,
        SellerModelError,
        SyncError
    };
    Q_ENUM(Error)

    using SellerId = int;
    using AdminId = int;

    explicit CommissionController(QObject *parent = nullptr);

    QAbstractItemModel *commissionModel() const;

    ProductInventory::ProductModel *selectedProducts() const;
    void setSelectedProducts(ProductInventory::ProductModel *newSelectedProducts);

    int supplierId() const;
    void setSupplierId(int newSupplierId);

    ProductInventory::ProductModel *productModel() const;
    void setProductModel(ProductInventory::ProductModel *newProductModel);

    QAbstractItemModel *sellerModel() const;
    void setSellerModel(QAbstractItemModel *newSellerModel);

    AdminId adminId() const;
    void setAdminId(AdminId newAdminId);

    // QQmlParserStatus interface
    void classBegin() override;
    void componentComplete() override;

    bool busy() const;
    void setBusy(bool newBusy);

public slots:
    void saveCommission(const QVariantMap &map);
    void fetchCommissions();
    void fetchCommissionsBySellerId(SellerId sellerId);

private slots:
    void networkStatusChanged(QNetworkInformation::Reachability reachability);
    void createSellerModel(QNetworkInformation::Reachability reachability);
    void createProductModel(QNetworkInformation::Reachability reachability);
    void createCommissionModel(QNetworkInformation::Reachability reachability);

    void onCommissionAdded();
    void onErrorOccurred();

signals:
    void selectedProductsChanged();

    void supplierIdChanged();

    void productModelChanged();

    void commissionModelChanged();

    void sellerModelChanged();

    void adminIdChanged();

    void commissionAdded();

    void databaseSynced();

    void errorOccurred(Error error, const QString &errorString);

    void busyChanged();

protected:
    void setCommissionModel(AbstractCommissionModel *newCommissionModel);

private:
    /**
     * @brief syncDatabase Keep in sync the products table in our local sqlite db.
     */
    void syncDatabase();

private:
    int supplierId_;
    AdminId adminId_;
    AbstractCommissionModel *commissionModel_;
    ProductInventory::ProductModel *productModel_;
    ProductInventory::ProductModel *selectedProducts_;
    QAbstractItemModel *sellerModel_;
    bool busy_;
};
}

#endif // COMMISSIONCONTROLLER_H
