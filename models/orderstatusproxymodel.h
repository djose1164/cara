#ifndef ORDERSTATUSPROXYMODEL_H
#define ORDERSTATUSPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QQmlEngine>

#include "orderentity.h"
#include "orderstatus.h"

using  EntityLayer::OrderEntity;

namespace ModelLayer {
class OrderStatusProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(OrderStatus::Status statusFilter READ statusFilter WRITE setStatusFilter NOTIFY statusFilterChanged FINAL)

public:
    explicit OrderStatusProxyModel(QObject *parent = nullptr);

    OrderStatus::Status statusFilter() const;
    void setStatusFilter(OrderStatus::Status newStatusFilter);

signals:
    void statusFilterChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    OrderStatus::Status statusFilter_;
};
}

#endif // ORDERSTATUSPROXYMODEL_H
