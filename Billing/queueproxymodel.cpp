#include "queueproxymodel.h"
#include "billmodel.h"

namespace Billing
{
QueueProxyModel::QueueProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
    , status_{OrderQueueEnum::FilterBy::Any}
    , filterEnabled_{true}
{}

bool QueueProxyModel::filterAcceptsRow(int source_row,
                                       const QModelIndex &source_parent) const
{
    if (!filterEnabled_)
        return true;

    QModelIndex index{sourceModel()->index(source_row, 0, source_parent)};
    using enum Model::BillModel::RoleNames;

    switch (status_)
    {
        using enum OrderQueueEnum::FilterBy;
    case NoAssignment:
        return index.data(int(TakenRole)).value<bool>() == false;
    case AssingedToMe:
        return index.data(int(TakenRole)).value<bool>();
    case Done:
        return index.data(int(DoneRole)).value<bool>();
    case Any:
        return status_ == Any;
    default:
        return false;
    }
}

bool QueueProxyModel::filterEnabled() const
{
    return filterEnabled_;
}

void QueueProxyModel::setFilterEnabled(bool newFilterEnabled)
{
    if (filterEnabled_ == newFilterEnabled)
        return;
    filterEnabled_ = newFilterEnabled;
    emit filterEnabledChanged();

    if (filterEnabled_)
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
