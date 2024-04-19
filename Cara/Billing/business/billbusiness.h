#ifndef BILLBUSINESS_H
#define BILLBUSINESS_H

#include "../Billing/model/billmodel.h"
#include "ibillrepository.h"
#include <QObject>
#include <QtQml>

namespace BusinessLayer {
using Model::BillModel;

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
    void fetchBillsByCustomerId(int customerId);
    void fetchBillById(int orderId);
    void fetchBillsByAdminId(int adminId);
    void fetchUnpaidBills();
    void fetchOrdersInQueue();

    void clearModel();

private slots:
    void handleRequestFinished();

signals:
    void modelChanged();
    void populated();
    void ordersNotFound();

private:
    DataLayer::IBillRepository *billRepository_;
    QNetworkReply *reply_;

    Q_OBJECT_BINDABLE_PROPERTY(BillBusiness, BillModel *, model_, &BillBusiness::modelChanged)
};
}

#endif // BILLBUSINESS_H
