#include "productmodel.h"

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

    const auto product{data_.at(index.row())};
    switch (static_cast<RoleNames>(role))
    {
    case RoleNames::SelectedQtyRole:
        product->setSelectedQty(value.toInt());
        emit dataChanged(QModelIndex(), QModelIndex(), {role});
        return true;
    default:
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
        return getValue->name();
    case RoleNames::ProductIdRole:
        return getValue->productId();
    case RoleNames::BuyPriceRole:
        return getValue->buyPrice();
    case RoleNames::SellPriceRole:
        return getValue->price();
    case RoleNames::DescriptionRole:
        return getValue->description();
    case RoleNames::ReorderPointRole:
        return getValue->reorderPoint();
    case RoleNames::QuantityAvailableRole:
        return getValue->quantityAvailable();
    case RoleNames::ImageUrlRole:
        return getValue->imageUrl();
    case RoleNames::SelectedQtyRole:
        return getValue->selectedQty();
    default:
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
    repository_ = newRepository;
    emit repositoryChanged();
}

int ProductModel::count() const
{
    return data_.count();
}

QObject *ProductModel::get(int index) const
{
    if (index < 0 || index >= data_.count())
        return nullptr;

    return data_.at(index);
}

bool ProductModel::contains(const EntityLayer::Product *product)
{
    return data_.contains(product);
}

void ProductModel::update(SalespersonId spId)
{
    if (!repository_)
    {
        qWarning("ProductModel::update: a repository needs to be set.");
        return;
    }

    repository_->fetchAll(spId);

    connect(repository_, &Business::IInventoryRepository::fetchedAll, this, [&](const QList<QPointer<EntityLayer::Product>> &products) {
        beginResetModel();
        data_.clear();
        data_ = products;
        endResetModel();
        emit countChanged();
    });
}

void ProductModel::append(QObject *product)
{
    beginInsertRows(QModelIndex(), count(), count());
    data_.append(qobject_cast<EntityLayer::Product *>(product));
    emit countChanged();
    endInsertRows();
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

QHash<int, QByteArray> ProductModel::roleNames() const
{
    static const QHash<int, QByteArray> hash{
        {int(RoleNames::ProductIdRole), "productId"},
        {int(RoleNames::ProductNameRole), "productName"},
        {int(RoleNames::DescriptionRole), "description"},
        {int(RoleNames::SellPriceRole), "sellPrice"},
        {int(RoleNames::ImageUrlRole), "imageUrl"},
        {int(RoleNames::QuantityAvailableRole), "quantityAvailable"},
        {int(RoleNames::ReorderPointRole), "reorderPoint"},
        {int(RoleNames::BuyPriceRole), "buyPrice"},
        {int(RoleNames::SelectedQtyRole), "selectedQty"}
    };
    return hash;
}
}
