#include "profitcalculator.h"
#include "Shopping/cataloguerepository.h"

using namespace Qt::StringLiterals;

namespace Billing
{

ProfitCalculator::ProfitCalculator(QObject *parent)
    : Calculator{parent}
    , catalogueRepository_{new Catalogue::CatalogueRepository{this}}
    , supplierId_{}
    , percentage_{1}
    , productList_{}
    , selectorModel_{}
    , grossTotal_{}
{
    connect(catalogueRepository_,
            &Catalogue::CatalogueRepository::productsFetched,
            this,
            [this](const QByteArray &data)
            {
                const auto productModel{qobject_cast<ProductInventory::ProductModel *>(model())};
                auto json{QJsonDocument::fromJson(data)};
                if (json["products"_L1].isArray())
                {
                    const auto productsArr = json["products"_L1].toArray();
                    for (const auto &p : productsArr)
                    {
                        productList_.append(EntityLayer::Product::fromJson(p.toObject()));
                    }

                    setBuyPriceInSelectorModel();
                }
            });

    connect(this, &ProfitCalculator::percentageChanged, this, &ProfitCalculator::calculate);

    supplierIdHandler_ = supplierId_.addNotifier(
        [this]
        {
            if (supplierId() > 0)
                catalogueRepository_->fetchProductsFromSupplier(supplierId_);
        });
}

ProfitCalculator::~ProfitCalculator()
{
    qDeleteAll(productList_);
    productList_.clear();
}

void ProfitCalculator::calculate()
{
    if (!model() or percentage_ < 1 or model()->rowCount() < 1 or productList_.count() < 1)
    {
        return;
    }

    const auto productModel{qobject_cast<ProductInventory::ProductModel *>(model())};
    const double calcPercentage{percentage_ / 100.};
    double calcTotal{};
    for (int i{}; i < model()->rowCount(); ++i)
    {
        using enum ProductInventory::ProductModel::RoleNames;

        //takeBuyPriceFromProductList(i);

        const EntityLayer::Product product{productModel->get(i)};
        const double sellPrice = product.price();
        const double buyPrice = product.buyPrice();
        const int qty = product.selectedQty();

        calcTotal += ((sellPrice - buyPrice) * qty);
    }

    grossTotal_ = calcTotal;
    setTotal(grossTotal_ * calcPercentage);
}

QList<EntityLayer::Product *> ProfitCalculator::productList() const
{
    return productList_;
}

void ProfitCalculator::setProductList(const QList<EntityLayer::Product *> &newProductList)
{
    productList_ = newProductList;
}

ProductInventory::ProductModel *ProfitCalculator::selectorModel() const
{
    return selectorModel_;
}

void ProfitCalculator::setSelectorModel(ProductInventory::ProductModel *newSelectorModel)
{
    if (selectorModel_ == newSelectorModel)
        return;
    if (selectorModel_)
    {
        disconnect(selectorModel_,
                   &QAbstractListModel::modelReset,
                   this,
                   &ProfitCalculator::setBuyPriceInSelectorModel);
    }

    selectorModel_ = newSelectorModel;

    if (selectorModel_)
    {
        connect(selectorModel_,
                &QAbstractListModel::modelReset,
                this,
                &ProfitCalculator::setBuyPriceInSelectorModel);
    }
}

double ProfitCalculator::grossTotal() const
{
    return grossTotal_;
}

void ProfitCalculator::setGrossTotal(double newGrossTotal)
{
    grossTotal_ = newGrossTotal;
}

QBindable<double> ProfitCalculator::bindableGrossTotal()
{
    return &grossTotal_;
}

int ProfitCalculator::percentage() const
{
    return percentage_;
}

void ProfitCalculator::setPercentage(int newPercentage)
{
    if (percentage_ == newPercentage)
        return;
    percentage_ = newPercentage;
    emit percentageChanged();
}

int ProfitCalculator::supplierId() const
{
    return supplierId_;
}

void ProfitCalculator::setSupplierId(int newSupplierId)
{
    if (supplierId_ == newSupplierId)
        return;
    supplierId_ = newSupplierId;
}

QBindable<int> ProfitCalculator::bindableSupplierId()
{
    return &supplierId_;
}

void ProfitCalculator::updateProductList()
{
    catalogueRepository_->fetchProductsFromSupplier(supplierId_);
}

void ProfitCalculator::takeBuyPriceFromProductList(int posInModel)
{
    using enum ProductInventory::ProductModel::RoleNames;

    const auto productModel{qobject_cast<ProductInventory::ProductModel *>(model())};
    auto &&product{productModel->get(posInModel)};
    if (product.buyPrice() > 0.0)
        return;

    const int productId{product.productId()};

    const auto found{findByProductId(productId)};
    if (found)
    {
        productModel->setData(productModel->find(productId),
                              (*found)->price(),
                              int(BuyPriceRole));
    }
}

void ProfitCalculator::setBuyPriceInSelectorModel()
{
    if (!selectorModel_ || productList_.count() == 0)
        return;

    for (size_t i{}; i < selectorModel_->count(); ++i)
    {
        auto &&product{selectorModel_->get(i)};
        const int productId{product.productId()};

        if (const std::optional<double> buyPrice{getBuyPriceByProductId(productId)}; buyPrice)
        {
            using enum ProductInventory::ProductModel::RoleNames;

            const QModelIndex productIndex{selectorModel_->find(productId)};
            selectorModel_->setData(productIndex, *buyPrice, int(BuyPriceRole));
        }
    }
}

std::optional<EntityLayer::Product *> ProfitCalculator::findByProductId(int productId)
{
    auto it{std::ranges::find_if(productList_,
                                 [=](const auto item) { return item->productId() == productId; })};
    if (it != productList_.end())
        return *it;

    return std::nullopt;
}

void ProfitCalculator::removeByProductId(int productId)
{
    productList_.removeIf([productId](const auto item) -> bool
                          {
                              return item->productId() == productId;
                          });
}

std::optional<double> ProfitCalculator::getBuyPriceByProductId(int productId)
{
    if (const auto fetched{findByProductId(productId)}; fetched)
    {
        return (*fetched)->price();
    }

    return std::nullopt;
}

} // namespace Billing



