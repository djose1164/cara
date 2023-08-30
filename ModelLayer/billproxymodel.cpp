#include "billproxymodel.h"
#include "billmodel.h"

BillProxyModel::BillProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
    , statusFilter_{PaymentEntity::Status::Any}
{

}

bool BillProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    using EntityLayer::PaymentEntity;
    const auto statusIdx{sourceModel()->index(source_row, 0, source_parent)};

    const auto payment{qvariant_cast<PaymentEntity *>(statusIdx.data(int(ModelLayer::BillModel::RoleNames::PaymentRole)))};
    const auto paymentStatus{payment->status()};

    return paymentStatus == statusFilter_ || statusFilter_ == PaymentEntity::Status::Any;
}

PaymentEntity::Status BillProxyModel::statusFilter() const
{
    return statusFilter_;
}

void BillProxyModel::setStatusFilter(const PaymentEntity::Status &newStatusFilter)
{
    if (statusFilter_ == newStatusFilter)
        return;
    statusFilter_ = newStatusFilter;
    emit statusFilterChanged();

    invalidateFilter();
}
