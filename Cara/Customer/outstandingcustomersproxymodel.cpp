#include "outstandingcustomersproxymodel.h"
#include "customeroutstandingordermodel.h"

namespace Customer
{
OutstandingCustomersProxyModel::OutstandingCustomersProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
    , ascending_{true}
    , customerName_{}
{
}

bool OutstandingCustomersProxyModel::filterAcceptsRow(int source_row,
                                                      const QModelIndex &source_parent) const
{
    Q_UNUSED(source_row);
    QModelIndex nameIdx{sourceModel()->index(source_row, 0, source_parent)};
    QString targetName{
        nameIdx.data(int(CustomerOutstandingOrderModel::RoleNames::ForenameRole))
            .toString()};
    return customerName_.size() < 3 || targetName.contains(customerName_, Qt::CaseInsensitive);
}

bool OutstandingCustomersProxyModel::lessThan(const QModelIndex &source_left,
                                              const QModelIndex &source_right) const
{
    return source_left < source_right == ascending_;
}

QString OutstandingCustomersProxyModel::customerName() const
{
    return customerName_;
}

void OutstandingCustomersProxyModel::setCustomerName(const QString &newCustomerName)
{
    if (customerName_ == newCustomerName)
        return;

    beginFilterChange();
    customerName_ = newCustomerName;
    invalidateRowsFilter();

    emit customerNameChanged();
}
}
