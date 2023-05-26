#ifndef BILLENTITY_H
#define BILLENTITY_H

#include "ordermodel.h"
#include "paymententity.h"
#include "userentity.h"
#include <QObject>
#include <QtQml>

namespace EntityLayer {
using ModelLayer::OrderModel;
class BillEntity : public QObject, public JsonExportable
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit BillEntity(QObject *parent = nullptr);

    int orderId() const;
    void setOrderId(int orderId);
    QBindable<int> bindableOrderId();

    UserEntity *user() const;
    void setUser(UserEntity *const user);

    OrderModel *details() const;
    void setDetailsModel(OrderModel *const model);

    PaymentEntity *payment() const;
    void setPayment(PaymentEntity *const payment);

    QDate date() const;
    void setDate(QDate date);
    QBindable<QDate> bindableDate();

    Q_INVOKABLE QByteArray toJson() const;

public slots:
    void appendDetail(DetailsEntity *const detail);

/*signals:
    void userChanged();
    void detailsChanged();
    void paymentChanged();
    void dateChanged();
    void orderIdChanged();*/

private:
    QProperty<int> orderId_;
    UserEntity * user_;
    OrderModel *details_;
    PaymentEntity * payment_;
    QProperty<QDate> date_;
};
}

#endif // BILLENTITY_H
