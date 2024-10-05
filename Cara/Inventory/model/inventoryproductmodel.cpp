#include "productmodel.h"
#include "iinventoryrepository.h"

using namespace Qt::StringLiterals;

namespace
{
constexpr auto imageUrlField{"imageUrl"_L1};
constexpr auto idField{"id"_L1};
}

namespace Model
{
ProductModel::ProductModel(QObject *parent)
    : QAbstractListModel{parent}
    , data_{}
    , repository_{}
{}


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
    case RoleNames::MinimumStockLevelRole:
        return {};
    }
}

Business::IInventoryRepository *ProductModel::repository() const
{
    return repository_;
}

void ProductModel::setRepository(Business::IInventoryRepository *newRepository)
{
    if (repository_ == newRepository)
        return;
    if (repository_)
        repository_->disconnect();

    repository_ = newRepository;

    if (repository_)
    {
        connect(repository_,
                &Business::IInventoryRepository::fetchedAll,
                this,
                [&](const QList<EntityLayer::Product> &products) { updateFromList(products); });
    }
    emit repositoryChanged();
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
    const auto pred = [productId](const EntityLayer::Product &p) -> bool {
        return p.productId() == productId;
    };

    if (const auto res{std::find_if(data_.begin(), data_.end(), pred)};
        res != data_.end())
    {
        if (const auto idx{data_.indexOf(std::as_const(*res))}; idx == -1)
            return {};
        else
            return index(idx);
    }

    return {};
}

void ProductModel::update(SalespersonId spId)
{
    if (!repository_)
    {
        qWarning("ProductModel::update: a repository needs to be set.");
        return;
    }

    repository_->fetchAll(spId);
}

void ProductModel::updateFromList(const QList<EntityLayer::Product> &products)
{
    beginResetModel();
    data_.clear();
    data_ = products;
    endResetModel();
    emit countChanged();
    emit updated();
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

QHash<int, QByteArray> ProductModel::roleNames() const
{
    static const QHash<int, QByteArray> hash{
        {int(RoleNames::ProductIdRole), "productId"},
        {int(RoleNames::ProductNameRole), "productName"},
        {int(RoleNames::DescriptionRole), "description"},
        {int(RoleNames::PriceRole), "price"},
        {int(RoleNames::BuyPriceRole), "buyPrice"},
        {int(RoleNames::ImageUrlRole), "imageUrl"},
        {int(RoleNames::QuantityAvailableRole), "quantityAvailable"},
        {int(RoleNames::ReorderPointRole), "reorderPoint"},
        {int(RoleNames::SelectedQtyRole), "selectedQty"},
        {int(RoleNames::CategoryIdRole), "categoryId"},
        {int(RoleNames::ImagesRole), "images"},
    };
    return hash;
}
}
