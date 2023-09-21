#ifndef BILLBUSINESS_H
#define BILLBUSINESS_H

#include "billmodel.h"
#include "rest.h"
#include <QObject>
#include <QtQml>

namespace BusinessLayer {
using EntityLayer::UserEntity, EntityLayer::BillEntity,
    ModelLayer::BillModel;

class BillBusiness : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(BillModel *model READ model WRITE setModel NOTIFY modelChanged BINDABLE bindableModel)

public:
    explicit BillBusiness(QObject *parent = nullptr);

    BillModel *model() const;
    void setModel(BillModel *const model);
    QBindable<BillModel *> bindableModel();

public slots:
    void populate();
    void fetchBillsByCustomerId(int customerId);
    void fetchBillById(int orderId);
    void fetchBillsByAdminId(int customerId);

    void clearModel();

signals:
    void modelChanged();
    void populated();

    void completed();

private:
    void fetch(QStringView arg);

    QProperty<Rest *> rest_;

    Q_OBJECT_BINDABLE_PROPERTY(BillBusiness, BillModel *, model_, &BillBusiness::modelChanged)
};
}

#endif // BILLBUSINESS_H
