#ifndef BILLENTITY_H
#define BILLENTITY_H

#include "../models/ordermodel.h"
#include "orderstatus.h"
#include "paymententity.h"
#include "../user/userentity.h"
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

    static BillEntity *fromMap(const QVariantMap &billMap);

    int orderId() const;
    void setOrderId(int orderId);
    QBindable<int> bindableOrderId();
    
    Entity::UserEntity user() const;
    void setUser(Entity::UserEntity user);

    OrderModel *details() const;
    void setDetailsModel(OrderModel *const model);

    PaymentEntity *payment() const;
    void setPayment(PaymentEntity *const payment);

    QDateTime date() const;
    void setDate(QDateTime date);
    QBindable<QDateTime> bindableDate();

    Q_INVOKABLE QByteArray toJson() const;

    OrderStatus *orderStatus() const;
    void setOrderStatus(OrderStatus *newOrderStatus);

public slots:
    void appendDetail(DetailsEntity *const detail);

private:
    QProperty<int> orderId_;
    Entity::UserEntity user_;
    OrderModel *details_;
    PaymentEntity * payment_;
    OrderStatus *orderStatus_;
    QProperty<QDateTime> date_;
};
}

#endif // BILLENTITY_H
