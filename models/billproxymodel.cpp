#include "billproxymodel.h"
#include "billmodel.h"

namespace ModelLayer {
using PaymentStatus = EntityLayer::PaymentEntity::Status;

BillProxyModel::BillProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
    , paymentFilter_{PaymentStatus::Any}
    , orderFilter_{OrderStatus::Status::Any}
{

}

bool BillProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    using EntityLayer::PaymentEntity;
    using RoleNames = ModelLayer::BillModel::RoleNames;
    const auto statusIdx{sourceModel()->index(source_row, 0, source_parent)};

    const auto filterPayment = [&] {
        const auto payment{qvariant_cast<PaymentEntity *>(
            statusIdx.data(int(RoleNames::PaymentRole))
        )};
        const auto paymentStatus{payment->status()};

        return paymentStatus == paymentFilter_ || paymentFilter_ == PaymentStatus::Any;
    };

    const auto filterOrder = [&] {
        const auto orderStatus{qvariant_cast<OrderStatus *>(
            statusIdx.data(int(RoleNames::StatusRole))
        )};

        return orderStatus->statusId() == int(orderFilter_) || orderFilter_ == OrderStatus::Status::Any;
    };

    return filterPayment() && filterOrder();
}

OrderStatus::Status BillProxyModel::orderFilter() const
{
    return orderFilter_;
}

void BillProxyModel::setOrderFilter(const OrderStatus::Status &newOrderFilter)
{
    if (orderFilter_ == newOrderFilter)
        return;
    orderFilter_ = newOrderFilter;
    emit orderFilterChanged();

    invalidateFilter();
}

PaymentStatus BillProxyModel::statusFilter() const
{
    return paymentFilter_;
}

void BillProxyModel::setStatusFilter(const PaymentStatus &newStatusFilter)
{
    if (paymentFilter_ == newStatusFilter)
        return;
    paymentFilter_ = newStatusFilter;
    emit statusFilterChanged();

    invalidateFilter();
}
}
