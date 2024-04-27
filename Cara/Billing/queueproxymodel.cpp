#include "queueproxymodel.h"
#include "billmodel.h"

using namespace Qt::StringLiterals;

namespace Billing
{
QueueProxyModel::QueueProxyModel(QObject *parent)
    : AbstractSortFilterModel{parent}
    , status_{OrderQueueEnum::FilterBy::Any}
    , searchString_{}
{}

bool QueueProxyModel::filterAcceptsRow(int source_row,
                                       const QModelIndex &source_parent) const
{
    if (!filterEnabled())
        return true;

    QModelIndex index{sourceModel()->index(source_row, 0, source_parent)};

    using enum Model::BillModel::RoleNames;

    const auto filteredStatus = [&] -> bool {
        switch (status_)
        {
            using enum OrderQueueEnum::FilterBy;
        case NoAssignment:
            return not index.data(int(TakenRole)).toBool();
        case AssingedToMe:
            return index.data(int(TakenRole)).toBool();
        case Done:
            return index.data(int(DoneRole)).toBool();
        case Any:
            return true;
        default:
            return false;
        }
    };

    const auto filteredString = [&] -> bool {
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

OrderQueueEnum::FilterBy QueueProxyModel::status() const
{
    return status_;
}

void QueueProxyModel::setStatus(OrderQueueEnum::FilterBy newStatus)
{
    if (status_ == newStatus)
        return;
    status_ = newStatus;
    emit statusChanged();

    invalidateFilter();
}
}
