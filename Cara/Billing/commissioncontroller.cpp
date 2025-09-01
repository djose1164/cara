#include "commissioncontroller.h"
#include "ProductInventory/productmodel.h"
#include "ProductInventory/sqliteproductrepository.h"
#include "ProductInventory/supplierproductrepository.h"
#include "Salesperson/salespersoncontroller.h"
#include "Salesperson/sellerlistmodel.h"
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
    , adminId_{-1}
    , commissionModel_{}
    , productModel_{new ProductInventory::ProductModel(this)}
    , selectedProducts_{new ProductInventory::ProductModel(this)}
    , sellerModel_{}
{
    if (QNetworkInformation::instance() == nullptr)
        QNetworkInformation::loadDefaultBackend();

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

CommissionController::AdminId CommissionController::adminId() const
{
    return adminId_;
}

void CommissionController::setAdminId(AdminId newAdminId)
{
    if (adminId_ == newAdminId)
        return;
    adminId_ = newAdminId;
    emit adminIdChanged();
}

QAbstractItemModel *CommissionController::sellerModel() const
{
    return sellerModel_;
}

void CommissionController::setSellerModel(QAbstractItemModel *newSellerModel)
{
    if (sellerModel_ == newSellerModel)
        return;
    if (sellerModel_)
    {
        sellerModel_->deleteLater();
        sellerModel_ = nullptr;
    }

    sellerModel_ = newSellerModel;
    emit sellerModelChanged();
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
    createSellerModel(reachability);
    if (reachability == QNetworkInformation::Reachability::Online)
    {
        if (commissionModel_ == nullptr or not dynamic_cast<CommissionModel *>(commissionModel_))
        {
            auto model{new CommissionModel(this)};
            auto repository{new RestCommissionRepository(model)};
            model->setRepository(repository);
            setCommissionModel(model);
            connect(model,
                    &CommissionModel::commissionAddedSuccessfully,
                    this,
                    &CommissionController::commissionAdded);
        }

        const auto productRepository{productModel_->repository()};
        if (not qobject_cast<ProductInventory::SupplierProductRepository* >(productRepository))
        {
            if (productRepository)
            {
                productRepository->deleteLater();
                productModel_->setRepository(nullptr);
            }

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
        auto sqliteModel{new SQLiteCommissionModel(this)};
        setCommissionModel(sqliteModel);

        if (productModel_->repository())
        {
            productModel_->repository()->deleteLater();
            productModel_->setRepository(nullptr);
        }

        productModel_->setRepository(new ProductInventory::SQLiteProductRepository(this));

        connect(sqliteModel,
                &SQLiteCommissionModel::commissionAddedSuccessfully,
                this,
                &CommissionController::commissionAdded);
    }
}

void CommissionController::createSellerModel(QNetworkInformation::Reachability reachability)
{
    if (reachability == QNetworkInformation::Reachability::Online)
    {
        if (sellerModel_ and qobject_cast<Seller::SellerListModel *>(sellerModel_))
            return;

        const auto sellerListModel{new Seller::SellerListModel(this)};
        const auto sellerController{new Salesperson::SalespersonController(sellerListModel)};
        sellerController->setAdminId(adminId_);
        sellerController->readSalespeople();
        setSellerModel(sellerListModel);

        connect(sellerController,
                &Salesperson::SalespersonController::fetched,
                sellerListModel,
                &Seller::SellerListModel::setSellers);
    }
    else
    {
        // Ofline model
        if (sellerModel_ and qobject_cast<SQLiteCommissionModel *>(sellerModel_))
            return;

        const auto sqliteModel{new SQLiteCommissionModel(this)};
        sqliteModel->refresh();
        setSellerModel(sqliteModel);
    }
}

void CommissionController::classBegin()
{
}

void CommissionController::componentComplete()
{
    networkStatusChanged(QNetworkInformation::instance()->reachability());
}

}
