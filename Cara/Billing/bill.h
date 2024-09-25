#ifndef BILLENTITY_H
#define BILLENTITY_H

#include <QObject>
#include <QtQml>
#include "Customer/entity/customer.h"
#include "model/ordermodel.h"
#include "orderstatus.h"
#include "payment.h"

namespace Entity {
using Model::OrderModel;
class Bill : public QObject
{
    Q_OBJECT
public:
    explicit Bill(QObject *parent = nullptr);

    static Bill *fromMap(const QVariantMap &billMap);

    int orderId() const;
    void setOrderId(int orderId);
    QBindable<int> bindableOrderId();

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

    bool isTaken() const;
    void setIsTaken(bool newIsTaken);

    bool done() const;
    void setDone(bool newDone);

    int quantityOfProducts() const;

    int totalToPay() const;

public slots:
    void appendDetail(EntityLayer::DetailsEntity *const detail);

private:
    QProperty<int> orderId_;
    QProperty<OrderModel *> details_;
    Entity::Payment payment_;
    OrderStatus *orderStatus_;
    QProperty<QDateTime> date_;
    Entity::Customer customer_;
    bool isTaken_;
    bool done_;
    QProperty<int> quantityOfProducts_;
    QProperty<int> totalToPay_;
};
}

#endif // BILLENTITY_H
