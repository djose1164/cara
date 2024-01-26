#ifndef BILLENTITY_H
#define BILLENTITY_H

#include "../billing/entity/payment.h"
#include "../billing/model/ordermodel.h"
#include "../Customer/entity/customer.h"
#include "orderstatus.h"
#include "../user/userentity.h"
#include <QObject>
#include <QtQml>

namespace EntityLayer {
using Model::OrderModel;
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

    Entity::Payment payment() const;
    void setPayment(const Entity::Payment &payment);

    QDateTime date() const;
    void setDate(QDateTime date);
    QBindable<QDateTime> bindableDate();

    Q_INVOKABLE QByteArray toJson() const;

    OrderStatus *orderStatus() const;
    void setOrderStatus(OrderStatus *newOrderStatus);

    Entity::Customer customer() const;
    void setCustomer(const Entity::Customer &newCustomer);

public slots:
    void appendDetail(DetailsEntity *const detail);

private:
    QProperty<int> orderId_;
    Entity::UserEntity user_;
    OrderModel *details_;
    Entity::Payment payment_;
    OrderStatus *orderStatus_;
    QProperty<QDateTime> date_;
    Entity::Customer customer_;
};
}

#endif // BILLENTITY_H
