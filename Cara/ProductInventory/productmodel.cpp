#include "productmodel.h"
#include "productrepository.h"
#include "validationerror.h"

using namespace Qt::StringLiterals;

namespace
{
constexpr auto imageUrlField{"imageUrl"_L1};
constexpr auto idField{"id"_L1};
}

namespace ProductInventory
{

ProductModel::ProductModel(QObject *parent)
    : ProductModel{nullptr, parent}
{
}

ProductModel::ProductModel(IProductRepository *newRepository, QObject *parent)
    : QAbstractListModel{parent}
    , data_{}
    , repository_{newRepository}
    , status_{Status::Idle}
{
    makeConnections();
}

int ProductModel::rowCount(const QModelIndex &parent) const
{
    return count();
}

bool ProductModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    auto &product{data_[index.row()]};
    switch (static_cast<RoleNames>(role))
    {
    case RoleNames::SelectedQtyRole:
        product.setSelectedQty(value.toInt());
        emit dataChanged(index, index, {role});
        return true;
    case RoleNames::QuantityAvailableRole:
        product.setQuantityAvailable(value.toInt());
        emit dataChanged(index, index, {role});
        return true;
    case RoleNames::CategoryIdRole:
        product.setCategoryId(value.toInt());
        emit dataChanged(index, index, {role});
        return true;
    case RoleNames::PriceRole:
        product.setPrice(value.toInt());
        emit dataChanged(index, index, {role});
        return true;
    case RoleNames::ProductNameRole:
        product.setName(value.toString());
        emit dataChanged(index, index, {role});
        return true;
    case RoleNames::ImagesRole:
        product.setImages(QJsonArray{});
        std::ranges::for_each(value.toList(), [&](const QVariant &item) {
            const auto itemMap{item.toMap()};
            QJsonObject obj;

            obj[imageUrlField] = itemMap[imageUrlField].toUrl().toString();
            if (itemMap.contains(idField))
                obj[idField] = itemMap[idField].toInt();

            product.images().append(std::move(obj));
        });

        emit dataChanged(index, index, {role});
        return true;
    case RoleNames::DescriptionRole:
        product.setDescription(value.toString());
        emit dataChanged(index, index, {role});
        return true;
    case RoleNames::BuyPriceRole:
        product.setBuyPrice(value.toInt());
        emit dataChanged(index, index, {role});
        return true;
    case RoleNames::ProductIdRole:
    case RoleNames::MaximumStockLevelRole:
    case RoleNames::MinimumStockLevelRole:
    case RoleNames::ReorderPointRole:
    case RoleNames::ImageUrlRole:
    case RoleNames::SupplierIdRole:
    case RoleNames::SupplierNameRole:
    case RoleNames::SupplierPriceRole:
        return false;
    case RoleNames::BuyPriceHistoryRole:
        product.setBuyPriceHistory(value.toJsonArray());
        emit dataChanged(index, index, {role});
        return true;
    case RoleNames::WarehouseNameRole:
        return false;
    }
}

QVariant ProductModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};
    const auto getValue{data_.at(index.row())};

    switch (static_cast<RoleNames>(role))
    {
    case RoleNames::ProductNameRole:
        return getValue.name();
    case RoleNames::ProductIdRole:
        return getValue.productId();
    case RoleNames::PriceRole:
        return getValue.price();
    case RoleNames::DescriptionRole:
        return getValue.description();
    case RoleNames::ReorderPointRole:
        return getValue.reorderPoint();
    case RoleNames::QuantityAvailableRole:
        return getValue.quantityAvailable();
    case RoleNames::ImageUrlRole:
        return getValue.imageUrl();
    case RoleNames::SelectedQtyRole:
        return getValue.selectedQty();
    case RoleNames::CategoryIdRole:
        return getValue.categoryId();
    case RoleNames::ImagesRole:
        return getValue.images();
    case RoleNames::BuyPriceRole:
        return getValue.buyPrice();
    case RoleNames::MaximumStockLevelRole:
        return getValue.maximumStockLevel();
    case RoleNames::MinimumStockLevelRole:
        return getValue.minimumStockLevel();
    case RoleNames::SupplierIdRole:
        return getValue.supplierCatalog()["supplier"_L1]["id"_L1].toInt();
    case RoleNames::SupplierNameRole:
        return getValue.supplierCatalog()["supplier"_L1]["name"_L1].toString();
    case RoleNames::SupplierPriceRole:
        return getValue.buyPrice();
    case RoleNames::BuyPriceHistoryRole:
        return getValue.buyPriceHistory();
    case RoleNames::WarehouseNameRole:
        return getValue.warehouseName();
    }
}

int ProductModel::count() const
{
    return data_.count();
}

EntityLayer::Product ProductModel::get(int index) const
{
    if (index < 0 || index >= data_.count())
        return {};

    return data_.at(index);
}

bool ProductModel::contains(const EntityLayer::Product &product) const
{
    return data_.contains(product);
}

QModelIndex ProductModel::find(int productId) const
{
    return find([productId](const EntityLayer::Product &p) -> bool {
        return p.productId() == productId;
    });
}

QModelIndex ProductModel::find(const std::function<bool(const EntityLayer::Product &)> &predicate) const
{
    const auto res{std::ranges::find_if(data_, predicate)};

    return res != data_.end() ? index(std::distance(data_.begin(), res)) : QModelIndex();
}

QJsonArray ProductModel::toJsonArray() const
{
    QJsonArray array;
    std::ranges::for_each(data_, [&](const auto &product) {
        QJsonObject json;
        json["product_id"_L1] = product.productId();
        json["quantity"_L1] = product.selectedQty();
        array.append(std::move(json));
    });
    return array;
}

void ProductModel::update()
{
    qDebug() << "ProductModel::update: " << repository_;
    if (!repository_)
    {
        qWarning("ProductModel::update: a repository needs to be set.");
        emit errorOccurred(Error::UpdateFailed, "A repository needs to be set."_L1);
        return;
    }

    qDebug() << "ProductModel::update: updating...";
    setStatus(Status::Busy);
    repository_->update();
}

void ProductModel::setProducts(const QList<EntityLayer::Product> &products)
{
    beginResetModel();
    data_.clear();
    data_ = products;
    endResetModel();
    emit countChanged();
}

void ProductModel::setProducts(QList<EntityLayer::Product> &&products)
{
    beginResetModel();
    data_.clear();
    data_ = std::move(products);
    endResetModel();
    emit countChanged();
}

void ProductModel::editFromList(const QVariantList &list) noexcept
{
    for (const auto &item : list)
        editFromJson(item.toJsonObject());
}

void ProductModel::editFromJson(const QJsonObject &json) noexcept
{
    if (not json.contains("product_id"_L1) || not json.contains("quantity"_L1))
        return;

    const int productId{json["product_id"_L1].toInt()};
    if (const auto fetched{find(productId)}; fetched.isValid())
    {
        const int currentQty{data(fetched, int(RoleNames::QuantityAvailableRole)).toInt()};
        const int newQty{currentQty + json["quantity"_L1].toInt()};
        setData(fetched, newQty, int(RoleNames::QuantityAvailableRole));
    }
}

ProductModel::Status ProductModel::status() const
{
    return status_;
}

void ProductModel::setStatus(Status newStatus)
{
    if (status_ == newStatus)
        return;
    status_ = newStatus;
    emit statusChanged();
}

void ProductModel::addProduct(const QVariantMap &map) noexcept
{
    try
    {
        ProductRepository::validateMap(map);
        const auto productRepository{new ProductRepository};
        connect(productRepository, &ProductRepository::added,
                this,
                [this, productRepository]
                {
                    emit added();
                    emit countChanged();
                    productRepository->deleteLater();
                });
        productRepository->add(map);
    }
    catch (const ValidationError &err)
    {
        qWarning() << "CatalogController::addProduct: " << err.what();
        //emit errorOccurred(Error::ValidationError, err.what());
    }
}

void ProductModel::append(const EntityLayer::Product &product)
{
    beginInsertRows(QModelIndex(), count(), count());
    data_.append(product);
    endInsertRows();
    emit countChanged();
}

void ProductModel::clear()
{
    beginResetModel();
    data_.clear();
    endResetModel();
    emit countChanged();
}

void ProductModel::remove(int index)
{
    if (index < 0 || index >= count())
        return;
    beginRemoveRows(QModelIndex(), index, index);
    data_.removeAt(index);
    endRemoveRows();
    emit countChanged();
}

void ProductModel::removeByProductId(ProductId id)
{
    if (const auto foundIdx{find(id)}; foundIdx.isValid())
        remove(foundIdx.row());
}

void ProductModel::refreshBuyPriceHistory(ProductId id) noexcept
{
    const auto productRepository{new ProductRepository};
    const auto reply{productRepository->read(id)};

    connect(reply, &QNetworkReply::finished,
            this, [this, id, reply] {
                QRestReply restReply{reply};
                if (not restReply.isSuccess())
                {
                    return;
                }

                const auto json{restReply.readJson()};
                if (not json)
                    return;

                if (QModelIndex &&found{find(id)};
                    found.isValid())
                {
                    const auto priceHistory{(*json)["product"_L1]["buy_price_history"_L1].toArray()};

                    setData(found, priceHistory, int(RoleNames::BuyPriceHistoryRole));

                    QJsonObject &&supplierCatalog{findLatestPrice(priceHistory)};
                    EntityLayer::Product &p{data_[found.row()]};
                    QJsonObject &&oldSC{p.supplierCatalog()["supplier"_L1].toObject()};
                    p.setSupplierCatalog(supplierCatalog);

                    using enum RoleNames;
                    auto supplierId{data(found, int(SupplierIdRole))};
                    auto supplierPrice{data(found, int(SupplierPriceRole))};
                    auto suppliername{data(found, int(SupplierNameRole))};

                    QList<int> changedRoles;
                    changedRoles.reserve(3); // Corresponding to Supplier*Role

                    if (supplierId.toInt() != oldSC["id"_L1].toInt())
                        changedRoles.append(int(SupplierIdRole));
                    if (supplierPrice != oldSC["name"_L1].toVariant())
                        changedRoles.append(int(SupplierPriceRole));
                    if (suppliername != p.supplierCatalog()["price"_L1].toVariant())
                        changedRoles.append(int(SupplierNameRole));

                    qDebug() << changedRoles;
                    emit dataChanged(found, found, changedRoles);
                }
                qDebug() << "ProductModel::refreshBuyPriceHistory: handling request...Done";
                emit buyPriceHistoryRefreshed();
            });

    connect(this, &ProductModel::buyPriceHistoryRefreshed,
            productRepository, [productRepository]{ productRepository->deleteLater(); });
}

void ProductModel::clearModel() noexcept
{
    beginResetModel();
    data_.clear();
    endResetModel();
    emit countChanged();
}

void ProductModel::productsFetched(const QList<EntityLayer::Product> &products) noexcept
{
    setProducts(products);
    setStatus(Status::Idle);
    emit updated();
    qDebug() << "ProductModel::productsFetched: adding products to list...count: " << products.count();
}

void ProductModel::makeConnections() noexcept
{
    if (repository_)
    {
        connect(repository_,
                &IProductRepository::fetched,
                this,
                &ProductModel::productsFetched);
        connect(repository_,
                &IProductRepository::errorOccurred,
                this,
                [this](auto err, const auto &errstr) {
                    if (err == IProductRepository::Error::FailedToFetch)
                        emit errorOccurred(Error::UpdateFailed, errstr);
                    else
                        emit errorOccurred(Error::UnknownError, errstr);
                });
    }
}

QJsonObject ProductModel::findLatestPrice(const QJsonArray &array) const noexcept
{
    auto it = std::ranges::find_if(array, [](const auto &item) -> bool {
        return item["thru_date"_L1].isNull();
    });

    return it != array.end() ? (*it).toObject() : QJsonObject{};
}

ProductInventory::IProductRepository *ProductModel::repository() const
{
    return repository_;
}

void ProductModel::setRepository(ProductInventory::IProductRepository *newRepository)
{
    if (repository_ == newRepository)
        return;
    if (repository_)
        repository_->disconnect(this);

    repository_ = newRepository;
    makeConnections();

    emit repositoryChanged();
}

QHash<int, QByteArray> ProductModel::roleNames() const
{
    return {
        {int(RoleNames::ProductIdRole), "productId"_ba},
        {int(RoleNames::ProductNameRole), "productName"_ba},
        {int(RoleNames::DescriptionRole), "description"_ba},
        {int(RoleNames::PriceRole), "price"_ba},
        {int(RoleNames::BuyPriceRole), "buyPrice"_ba},
        {int(RoleNames::ImageUrlRole), "imageUrl"_ba},
        {int(RoleNames::QuantityAvailableRole), "quantityAvailable"_ba},
        {int(RoleNames::ReorderPointRole), "reorderPoint"_ba},
        {int(RoleNames::SelectedQtyRole), "selectedQty"_ba},
        {int(RoleNames::CategoryIdRole), "categoryId"_ba},
        {int(RoleNames::ImagesRole), "images"_ba},
        {int(RoleNames::SupplierIdRole), "supplierId"_ba},
        {int(RoleNames::SupplierNameRole), "supplierName"_ba},
        {int(RoleNames::SupplierPriceRole), "supplierPrice"_ba},
        {int(RoleNames::BuyPriceHistoryRole), "buyPriceHistory"_ba},
        {int(RoleNames::MaximumStockLevelRole), "maximunStockLevel"_ba},
        {int(RoleNames::MinimumStockLevelRole), "minimunStockLevel"_ba},
        {int(RoleNames::WarehouseNameRole), "warehouseName"_ba}
    };
}

}
