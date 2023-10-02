#ifndef BILLPROXYMODEL_H
#define BILLPROXYMODEL_H

#include "paymententity.h"

#include <QSortFilterProxyModel>
#include <QObject>
#include <QQmlEngine>

namespace ModelLayer {
class BillProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    QML_ELEMENT

    using PaymentStatus = EntityLayer::PaymentEntity::Status;

    Q_PROPERTY(PaymentStatus statusFilter READ statusFilter WRITE setStatusFilter NOTIFY statusFilterChanged)

public:
    explicit BillProxyModel(QObject *parent = nullptr);

    PaymentStatus statusFilter() const;
    void setStatusFilter(const PaymentStatus &newStatusFilter);

signals:
    void statusFilterChanged();

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    PaymentStatus statusFilter_;
};
}

#endif // BILLPROXYMODEL_H
