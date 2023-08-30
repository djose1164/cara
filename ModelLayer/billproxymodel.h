#ifndef BILLPROXYMODEL_H
#define BILLPROXYMODEL_H

#include "paymentbusiness.h"

#include <QSortFilterProxyModel>
#include <QObject>
#include <QQmlEngine>

namespace ModelLayer {
class BillProxyModel;
}

class BillProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(PaymentEntity::Status statusFilter READ statusFilter WRITE setStatusFilter NOTIFY statusFilterChanged)

public:
    explicit BillProxyModel(QObject *parent = nullptr);

    PaymentEntity::Status statusFilter() const;
    void setStatusFilter(const PaymentEntity::Status &newStatusFilter);

signals:
    void statusFilterChanged();

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    PaymentEntity::Status statusFilter_;
};

#endif // BILLPROXYMODEL_H
