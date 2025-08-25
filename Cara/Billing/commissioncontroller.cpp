#include "commissioncontroller.h"
#include "ProductInventory/productmodel.h"
#include "ProductInventory/sqliteproductrepository.h"
#include "ProductInventory/supplierproductrepository.h"
#include "abstractcommissionmodel.h"
#include "commissionmodel.h"
#include "restcommissionrepository.h"
#include "sqlitecommissionmodel.h"

#include <QNetworkInformation>


namespace Billing
{
CommissionController::CommissionController(QObject *parent)
    : QObject{parent}
    , supplierId_{-1}
    , commissionModel_{}
    , productModel_{new ProductInventory::ProductModel(this)}
    , selectedProducts_{new ProductInventory::ProductModel(this)}
{
    if (QNetworkInformation::instance() == nullptr)
        QNetworkInformation::loadDefaultBackend();

    networkStatusChanged(QNetworkInformation::instance()->reachability());

    connect(QNetworkInformation::instance(),
            &QNetworkInformation::reachabilityChanged,
            this,
            &CommissionController::networkStatusChanged);
}

QAbstractItemModel *CommissionController::commissionModel() const
{
    return commissionModel_->itemModel();
}

void CommissionController::setCommissionModel(AbstractCommissionModel *newCommissionModel)
{
    if (commissionModel_ == newCommissionModel)
        return;
    if (commissionModel_)
    {
        commissionModel_->scheduleDelete();
        commissionModel_ = nullptr;
    }
    commissionModel_ = newCommissionModel;

    emit commissionModelChanged();
}

ProductInventory::ProductModel *CommissionController::productModel() const
{
    return productModel_;
}

void CommissionController::setProductModel(ProductInventory::ProductModel *newProductModel)
{
    if (productModel_ == newProductModel)
        return;
    productModel_ = newProductModel;
    emit productModelChanged();
}

int CommissionController::supplierId() const
{
    return supplierId_;
}

void CommissionController::setSellerId(int newSellerId)
{
    if (supplierId_ == newSellerId)
        return;
    supplierId_ = newSellerId;
    emit supplierIdChanged();
}

ProductInventory::ProductModel *CommissionController::selectedProducts() const
{
    return selectedProducts_;
}

void CommissionController::setSelectedProducts(ProductInventory::ProductModel *newSelectedProducts)
{
    if (selectedProducts_ == newSelectedProducts)
        return;
    selectedProducts_ = newSelectedProducts;
    emit selectedProductsChanged();
}

void CommissionController::append(const QVariantMap &map)
{
    if (commissionModel_)
        commissionModel_->append(map);
}

void CommissionController::fetchCommissions()
{
    if (commissionModel_)
        commissionModel_->fetchCommissions();
}

void CommissionController::fetchCommissionsBySellerId(SellerId sellerId)
{
    if (commissionModel_)
        commissionModel_->fetchCommissionsBySellerId(sellerId);
}

void CommissionController::networkStatusChanged(QNetworkInformation::Reachability reachability)
{
    qDebug() << "CommissionController::CommissionController: connect to QNetworkInformation";
    if (reachability == QNetworkInformation::Reachability::Online)
    {
        if (commissionModel_ == nullptr or not dynamic_cast<CommissionModel *>(commissionModel_))
        {
            auto model{new CommissionModel(this)};
            auto repository{new RestCommissionRepository(model)};
            model->setRepository(repository);
            setCommissionModel(model);
        }

        const auto productRepository{productModel_->repository()};
        if (not qobject_cast<ProductInventory::SupplierProductRepository* >(productRepository))
        {
            if (productRepository)
                productRepository->deleteLater();

            auto *repository{new ProductInventory::SupplierProductRepository(this)};
            repository->setSupplierId(supplierId_);
            productModel_->setRepository(repository);
            connect(this, &CommissionController::supplierIdChanged, repository, [repository = std::move(repository), this] {
                repository->setSupplierId(supplierId_);
            });
        }
    }
    else
    {
        setCommissionModel(new SQLiteCommissionModel(this));
        if (productModel_->repository())
            productModel_->repository()->deleteLater();
        productModel_->setRepository(new ProductInventory::SQLiteProductRepository(this));
    }
}
}
