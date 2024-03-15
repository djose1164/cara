#ifndef CARTBUSINESS_H
#define CARTBUSINESS_H

#include "../data/rest.h"
#include "userentity.h"
#include "orderentity.h"
#include "../Billing/model/ordermodel.h"

#include <QObject>
#include <QtQml>

namespace BusinessLayer {

class CartBusiness : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(Entity::UserEntity user READ user WRITE setUser NOTIFY userChanged)
    Q_PROPERTY(EntityLayer::OrderEntity *order READ order WRITE setOrder NOTIFY orderChanged)
    Q_PROPERTY(Model::OrderModel *model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(int totalToPay READ totalToPay NOTIFY totalToPayChanged)

public:
    explicit CartBusiness(QObject *parent = nullptr);
    
    Entity::UserEntity user() const;
    EntityLayer::OrderEntity *order() const;

    Model::OrderModel *model() const;
    
    Q_INVOKABLE bool isProductInCart(EntityLayer::Product *const product);
    Q_INVOKABLE int totalToPay() const;

public slots:
    void setUser(Entity::UserEntity user);
    void setOrder(EntityLayer::OrderEntity *const order);
    void setModel(Model::OrderModel *const model);

    void addtoCart(EntityLayer::Product *const detail);
    void removeFromCart(EntityLayer::DetailsEntity *const detail);

    /**
     * @brief checkout Create an order and send it to the server. This method
     * do some checking process.
     */
    void checkout();


signals:
    void userChanged();
    void modelChanged();
    void orderChanged();
    void checkoutSucceeded();
    void checkoutFailed(const QVariantMap &resp);
    void totalToPayChanged();

private:
    void prepareOrder();

    Rest *rest_;
    
    Entity::UserEntity user_;
    EntityLayer::OrderEntity *order_;
    Model::OrderModel *model_;

    int totalToPay_;
};
}

#endif // CARTBUSINESS_H
