#include "queueproxymodel.h"
#include "orderqueuemodel.h"

using namespace Qt::StringLiterals;

namespace Billing
{
QueueProxyModel::QueueProxyModel(QObject *parent)
    : AbstractSortFilterModel{parent}
    , status_{OrderQueue::Status::Any}
    , searchString_{}
{}

bool QueueProxyModel::filterAcceptsRow(int source_row,
                                       const QModelIndex &source_parent) const
{
    if (!filterEnabled())
        return true;

    QModelIndex index{sourceModel()->index(source_row, 0, source_parent)};

    using enum OrderQueueModel::RoleNames;

    const auto filteredStatus = [&]() -> bool {
        return status_ == OrderQueue::Status::Any
               or index.data(int(QueueStatusIdRole)).toInt() == int(status_);
    };

    const auto filteredString = [&]() -> bool {
        auto &&customer{index.data(int(CustomerRole)).value<Entity::Customer>()};
        const auto customerName{customer.forename() + " "_L1 + customer.surname()};
        return customerName.contains(searchString_, Qt::CaseInsensitive) || searchString_.isEmpty();
    };

    return filteredString() and filteredStatus();
}

QString QueueProxyModel::searchString() const
{
    return searchString_;
}

void QueueProxyModel::setSearchString(const QString &newSearchString)
{
    if (searchString_ == newSearchString)
        return;
    searchString_ = newSearchString;
    emit searchStringChanged();

    invalidateFilter();
}

OrderQueue::Status QueueProxyModel::status() const
{
    return status_;
}

void QueueProxyModel::setStatus(OrderQueue::Status newStatus)
{
    if (status_ == newStatus)
        return;
    status_ = newStatus;
    emit statusChanged();

    invalidateFilter();
}
}
