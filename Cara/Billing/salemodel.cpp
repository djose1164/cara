#include "salemodel.h"
#include "salerepository.h"

using namespace Qt::StringLiterals;

namespace
{
const auto saleId{"saleId"_ba};
const auto orderId{"orderId"_ba};
const auto customerId{"customerId"_ba};
const auto saleItems{"saleItems"_ba};
const auto totalAmount{"totalAmount"_ba};
const auto createdAt{"createdAt"_ba};
}

namespace Billing
{

SaleModel::SaleModel(QObject *parent)
    : QAbstractListModel{parent}
    , data_{}
    , repository_{}
    , totalPages_{}
    , isFetching_{}
{
    pagination_.setPerPage(1);
}

int SaleModel::rowCount(const QModelIndex &parent) const
{
    return data_.count();
}

QVariant SaleModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    const Sale &sale{data_.at(index.row())};
    switch (static_cast<RoleNames>(role))
    {
    case RoleNames::SaleIdRole:
        return sale.saleId();
    case RoleNames::OrderIdRole:
        return sale.orderId();
    case RoleNames::CustomerIdRole:
        return sale.customerId();
    case RoleNames::SaleItemsRole:
        return QVariant::fromValue(sale.saleItems());
    case RoleNames::TotalAmountRole:
        return sale.totalAmount();
    case RoleNames::CreatedAtRole:
        return sale.createdAt();
    }
}

bool SaleModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}

QHash<int, QByteArray> SaleModel::roleNames() const
{
    using enum RoleNames;
    return {{int(SaleIdRole), saleId},
            {int(OrderIdRole), orderId},
            {int(CustomerIdRole), customerId},
            {int(SaleItemsRole), saleItems},
            {int(TotalAmountRole), totalAmount},
            {int(CreatedAtRole), createdAt}};
}

bool SaleModel::canFetchMore(const QModelIndex &parent) const
{
    const bool isPossible{pagination_.total() == 0 or pagination_.canFetchMore()};
    qDebug() << "SaleModel::canFetchMore: " << isPossible;
    qDebug() << "SaleModel::canFetchMore: " << pagination_.total() << "/" << pagination_.pages();
             //<< (pagination_.total() / pagination_.pages());
    return !isFetching_ and isPossible;
}

void SaleModel::fetchMore(const QModelIndex &parent)
{
    if (!pagination_.isValid())
        return;
    qDebug() << "SaleModel::fetchMore: fetching more...";
    // Fetch more from api

    isFetching_ = true;
    repository_->fetchSales();
}

void SaleModel::validateSale(const QVariantMap &sale)
{
    constexpr auto errorTemplate{"SaleModel::validateSale: Missing argument: %1"_L1};
    if (!sale.contains(customerId))
        throw std::invalid_argument{errorTemplate.arg(customerId).toUtf8()};
    else if (!sale.contains(saleItems))
        throw std::invalid_argument{errorTemplate.arg(saleItems).toUtf8()};
    for (const auto &item : sale[saleItems].toList())
    {
        qDebug() << "item: " << item;

        const auto itemMap{item.toMap()};
        constexpr auto productId{"productId"_L1};
        constexpr auto quantity{"quantity"_L1};

        if (!itemMap.contains(productId))
            throw std::invalid_argument{errorTemplate.arg(productId).toUtf8()};
        else if (!itemMap.contains(quantity))
            throw std::invalid_argument{errorTemplate.arg(quantity).toUtf8()};
    }

}

void SaleModel::append(const QVariantMap &sale)
{
    try
    {
        validateSale(sale);
    }
    catch (const std::invalid_argument &err)
    {
        const auto errorStr{err.what()};
        qWarning() << errorStr;
        emit errorOccurred(errorStr);
    }
}

void SaleModel::fetchSales()
{
    if (!repository_)
        return;
    repository_->fetchSales();
}

void SaleModel::salesFetched(const QList<Sale> &sales, const Network::Pagination &pagination)
{
    qDebug() << "SaleModel::salesFetched: pagination.isValid()" << pagination.isValid();
    setPagination(pagination);
    qDebug() << "SaleModel::salesFetched: " << pagination_;

    if (pagination.isValid())
    {
        beginInsertRows(QModelIndex{}, data_.count(), data_.count() + pagination_.perPage() - 1);
        std::for_each(sales.begin(),
                      sales.end(),
                      [this](const auto &sale) { data_.append(sale); });
        endInsertRows();
    }
    else
    {
        beginResetModel();
        data_.clear();
        data_ = std::move(sales);
        endResetModel();
    }

    isFetching_ = false;
}

void SaleModel::saleAdded(const QJsonObject &json)
{
    const auto count{data_.count()};
    beginInsertRows(QModelIndex{}, count, count);
    data_.append(Sale::fromJson(json));
    endInsertRows();
}

Network::Pagination SaleModel::pagination() const
{
    return pagination_;
}

void SaleModel::setPagination(const Network::Pagination &newPagination)
{
    pagination_ = newPagination;
}

SaleRepository *SaleModel::repository() const
{
    return repository_;
}

void SaleModel::setRepository(SaleRepository *newRepository)
{
    if (repository_ == newRepository)
        return;
    if (repository_)
        repository_->disconnect(this);

    repository_ = newRepository;

    if (repository_)
    {
        repository_->setPagination(pagination_);
        connect(repository_, &SaleRepository::fetched, this, &SaleModel::salesFetched);
    }
    emit repositoryChanged();
}

} // namespace Billing
