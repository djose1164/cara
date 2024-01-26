#ifndef BILLPROXYMODEL_H
#define BILLPROXYMODEL_H

#include "orderstatus.h"
#include "entity/payment.h"
#include <QSortFilterProxyModel>
#include <QObject>
#include <QQmlEngine>

namespace ModelLayer {
class BillProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    QML_ELEMENT

    using PaymentStatus = Entity::Payment::Status;

    Q_PROPERTY(PaymentStatus paymentFilter READ statusFilter WRITE setStatusFilter NOTIFY statusFilterChanged FINAL)
    Q_PROPERTY(OrderStatus::Status orderFilter READ orderFilter WRITE setOrderFilter NOTIFY orderFilterChanged FINAL)

public:
    explicit BillProxyModel(QObject *parent = nullptr);

    PaymentStatus statusFilter() const;
    void setStatusFilter(const PaymentStatus &newStatusFilter);

    OrderStatus orderStatus() const;
    void setOrderStatus(const OrderStatus &newOrderStatus);

    OrderStatus::Status orderFilter() const;
    void setOrderFilter(const OrderStatus::Status &newOrderFilter);

signals:
    void statusFilterChanged();

    // QSortFilterProxyModel interface
    void orderStatusChanged();

    void orderFilterChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    PaymentStatus paymentFilter_;
    OrderStatus::Status orderFilter_;
};
}

#endif // BILLPROXYMODEL_H
