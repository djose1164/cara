#include "orderstatusproxymodel.h"
#include "billmodel.h"

namespace ModelLayer {
using namespace EntityLayer;
OrderStatusProxyModel::OrderStatusProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
    , statusFilter_{OrderStatus::Status::Any}
{

}

bool OrderStatusProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const auto statusIdx{sourceModel()->index(source_row, 0, source_parent)};

    using ModelLayer::BillModel;
    const auto orderStatus{qvariant_cast<OrderStatus *>(
        statusIdx.data(int(BillModel::RoleNames::StatusRole))
    )};

    return orderStatus->statusId() == int(statusFilter_) || statusFilter_ == OrderStatus::Status::Any;
}

OrderStatus::Status OrderStatusProxyModel::statusFilter() const
{
    return statusFilter_;
}

void OrderStatusProxyModel::setStatusFilter(OrderStatus::Status newStatusFilter)
{
    if (statusFilter_ == newStatusFilter)
        return;
    statusFilter_ = newStatusFilter;
    emit statusFilterChanged();

    invalidateFilter();
}
}
