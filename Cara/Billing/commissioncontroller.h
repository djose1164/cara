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
    Q_PROPERTY(ProductInventory::ProductModel *productModel READ productModel WRITE setProductModel
                   NOTIFY productModelChanged FINAL)
    Q_PROPERTY(QAbstractItemModel *commissionModel READ commissionModel NOTIFY commissionModelChanged FINAL)
    Q_PROPERTY(ProductInventory::ProductModel *selectedProducts READ selectedProducts WRITE
                   setSelectedProducts NOTIFY selectedProductsChanged FINAL)
    Q_PROPERTY(int supplierId READ supplierId WRITE setSellerId NOTIFY supplierIdChanged FINAL)
    Q_PROPERTY(QAbstractItemModel *sellerModel READ sellerModel WRITE setSellerModel NOTIFY
                   sellerModelChanged FINAL)
    Q_PROPERTY(int adminId READ adminId WRITE setAdminId NOTIFY adminIdChanged FINAL REQUIRED)
    QML_ELEMENT
public:
    enum class Error
    {
        ProductModelError,
        CommissionModelError,
        SellerModelError,
    };
    Q_ENUM(Error)

    using SellerId = int;
    using AdminId = int;

    explicit CommissionController(QObject *parent = nullptr);

    QAbstractItemModel *commissionModel() const;

    ProductInventory::ProductModel *selectedProducts() const;
    void setSelectedProducts(ProductInventory::ProductModel *newSelectedProducts);

    int supplierId() const;
    void setSellerId(int newSellerId);

    ProductInventory::ProductModel *productModel() const;
    void setProductModel(ProductInventory::ProductModel *newProductModel);

    QAbstractItemModel *sellerModel() const;
    void setSellerModel(QAbstractItemModel *newSellerModel);

    AdminId adminId() const;
    void setAdminId(AdminId newAdminId);

    // QQmlParserStatus interface
    void classBegin() override;
    void componentComplete() override;

public slots:
    void append(const QVariantMap &map);
    void fetchCommissions();
    void fetchCommissionsBySellerId(SellerId sellerId);

private slots:
    void networkStatusChanged(QNetworkInformation::Reachability reachability);
    void createSellerModel(QNetworkInformation::Reachability reachability);

signals:
    void selectedProductsChanged();

    void supplierIdChanged();

    void productModelChanged();

    void commissionModelChanged();

    void sellerModelChanged();

    void adminIdChanged();

    void commissionAdded();

    void errorOccurred(Error error, const QString &errorString);

protected:
    void setCommissionModel(AbstractCommissionModel *newCommissionModel);

private:
    int supplierId_;
    AdminId adminId_;
    AbstractCommissionModel *commissionModel_;
    ProductInventory::ProductModel *productModel_;
    ProductInventory::ProductModel *selectedProducts_;
    QAbstractItemModel *sellerModel_;
};
}

#endif // COMMISSIONCONTROLLER_H
