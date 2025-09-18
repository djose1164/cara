#include "commissioncontroller.h"
#include "ProductInventory/productmodel.h"
#include "ProductInventory/supplierproductrepository.h"
#include "Salesperson/salespersoncontroller.h"
#include "Salesperson/sellerlistmodel.h"
#include "abstractcommissionmodel.h"
#include "commissionmodel.h"
#include "restcommissionrepository.h"
#include "sellerproductrepositoryrest.h"
#include "sqlitecommissionmodel.h"
#include "sqlitedatabase.h"
#include "sqliteproductrepository.h"
#include "sqlitesellermodel.h"

#include <QNetworkInformation>

constexpr int supplierIdToSync{1};


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
    , busy_{}
{
    QNetworkInformation::loadDefaultBackend();

    connect(QNetworkInformation::instance(),
            &QNetworkInformation::reachabilityChanged,
            this,
            &CommissionController::networkStatusChanged);

    connect(this,
            &CommissionController::errorOccurred,
            this,
            &CommissionController::onErrorOccurred);
    connect(this,
            &CommissionController::commissionAdded,
            this,
            &CommissionController::onCommissionAdded);
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

void CommissionController::syncDatabase()
{
    if (not ProductInventory::SQLiteDatabase::needToSync())
        return;

    // Fetch product list and save it.
    std::unique_ptr<ProductInventory::IProductRepository> repository{
        std::make_unique<ProductInventory::SellerProductRepositoryREST>(supplierIdToSync)};

    repository->update();

    connect(repository.get(),
            &ProductInventory::IProductRepository::fetched,
            this,
            [this, r = std::move(repository)](const auto &productList) {
                if (ProductInventory::SQLiteDatabase::resetProducts(productList))
                    emit databaseSynced();
                else
                    emit errorOccurred(Error::SyncError, "Couldn't sync database!");
            });
}

bool CommissionController::busy() const
{
    return busy_;
}

void CommissionController::setBusy(bool newBusy)
{
    if (busy_ == newBusy)
        return;
    busy_ = newBusy;
    emit busyChanged();
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

void CommissionController::setSupplierId(int newSupplierId)
{
    if (supplierId_ == newSupplierId)
        return;
    supplierId_ = newSupplierId;
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

void CommissionController::saveCommission(const QVariantMap &map)
{
    if (commissionModel_)
    {
        setBusy(true);
        commissionModel_->append(map);
    }
}

void CommissionController::fetchCommissions()
{
    if (commissionModel_)
    {
        setBusy(true);
        commissionModel_->fetchCommissions();
    }
}

void CommissionController::fetchCommissionsBySellerId(SellerId sellerId)
{
    if (commissionModel_)
    {
        setBusy(true);
        commissionModel_->fetchCommissionsBySellerId(sellerId);
    }
}

void CommissionController::networkStatusChanged(QNetworkInformation::Reachability reachability)
{
    qDebug() << "CommissionController::networkStatusChanged: " << reachability;
    createSellerModel(reachability);
    createProductModel(reachability);
    createCommissionModel(reachability);
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
        if (sellerModel_ and qobject_cast<Seller::SQLiteSellerModel *>(sellerModel_))
            return;

        const auto sqliteModel{new Seller::SQLiteSellerModel(this)};
        sqliteModel->refresh();
        setSellerModel(sqliteModel);
    }
}

void CommissionController::createProductModel(QNetworkInformation::Reachability reachability)
{
    if (reachability == QNetworkInformation::Reachability::Online)
    {
        // online
        const auto productRepository{productModel_->repository()};
        if (not qobject_cast<ProductInventory::SupplierProductRepository *>(productRepository))
        {
            if (productRepository)
            {
                productRepository->deleteLater();
                productModel_->setRepository(nullptr);
            }

            auto *repository{new ProductInventory::SupplierProductRepository(this)};
            repository->setSupplierId(supplierId_);
            productModel_->setRepository(repository);

            connect(this,
                    &CommissionController::supplierIdChanged,
                    repository,
                    [repository = std::move(repository), this] {
                        repository->setSupplierId(supplierId_);
                    });
            connect(productModel_,
                    &ProductInventory::ProductModel::errorOccurred,
                    this,
                    [this](auto err, const QString &errstr) {
                        Q_UNUSED(err)
                        emit errorOccurred(Error::ProductModelError, errstr);
                    });
        }
    }
    else
    {
        // offline
        if (productModel_->repository())
        {
            productModel_->repository()->deleteLater();
            productModel_->setRepository(nullptr);
        }

        productModel_->setRepository(new ProductInventory::SQLiteProductRepository(this));

        connect(productModel_,
                &ProductInventory::ProductModel::errorOccurred,
                this,
                [this](auto err, const QString &errstr) {
                    Q_UNUSED(err)
                    emit errorOccurred(Error::ProductModelError, errstr);
                });
    }
}

void CommissionController::createCommissionModel(QNetworkInformation::Reachability reachability)
{
    if (reachability == QNetworkInformation::Reachability::Online)
    {
        if (commissionModel_ == nullptr or not dynamic_cast<CommissionModel *>(commissionModel_))
        {
            auto model{new CommissionModel(this)};
            auto repository{new RestCommissionRepository(model)};
            model->setRepository(repository);

            setCommissionModel(model);

            connect(model, &CommissionModel::modelUpdated, this, [this] { setBusy(false); });
            connect(model,
                    &CommissionModel::commissionAddedSuccessfully,
                    this,
                    &CommissionController::commissionAdded);
            connect(model,
                    &CommissionModel::errorOccurred,
                    this,
                    [this](auto err, const auto &errstr) {
                        Q_UNUSED(err)
                        emit errorOccurred(Error::CommissionModelError, errstr);
                    });
        }
    }
    else
    {
        auto sqliteModel{new SQLiteCommissionModel(this)};
        setCommissionModel(sqliteModel);

        connect(sqliteModel,
                &SQLiteCommissionModel::commissionAddedSuccessfully,
                this,
                &CommissionController::commissionAdded);
        connect(sqliteModel,
                &SQLiteCommissionModel::errorOccurred,
                this,
                [this](SQLiteCommissionModel::Error err, const QString &errstr) {
                    Q_UNUSED(err)
                    emit errorOccurred(Error::CommissionModelError, errstr);
                });
    }
}

void CommissionController::onCommissionAdded()
{
    setBusy(false);
}

void CommissionController::onErrorOccurred()
{
    setBusy(false);
}

void CommissionController::classBegin()
{
}

void CommissionController::componentComplete()
{
    const auto reachability{QNetworkInformation::instance()->reachability()};
    networkStatusChanged(reachability);

    if (reachability == QNetworkInformation::Reachability::Online)
        syncDatabase();
}

}
