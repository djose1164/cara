#ifndef CARTBUSINESS_H
#define CARTBUSINESS_H

#include "rest.h"
#include "userentity.h"
#include "orderentity.h"
#include "ordermodel.h"

#include <QObject>
#include <QtQml>

namespace BusinessLayer {
class CartBusiness : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(EntityLayer::UserEntity *user READ user WRITE setUser NOTIFY userChanged)
    Q_PROPERTY(EntityLayer::OrderEntity *order READ order WRITE setOrder NOTIFY orderChanged)
    Q_PROPERTY(ModelLayer::OrderModel *model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(int totalToPay READ totalToPay NOTIFY totalToPayChanged)

public:
    explicit CartBusiness(QObject *parent = nullptr);

    EntityLayer::UserEntity *user() const;
    EntityLayer::OrderEntity *order() const;

    ModelLayer::OrderModel *model() const;

    Q_INVOKABLE bool isProductInCart(EntityLayer::ProductEntity *const product);
    Q_INVOKABLE int totalToPay() const;

public slots:
    void setUser(EntityLayer::UserEntity *user);
    void setOrder(EntityLayer::OrderEntity *const order);
    void setModel(ModelLayer::OrderModel *const model);

    void addtoCart(EntityLayer::DetailsEntity *const detail);
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
    void totalToPayChanged();

private:
    void prepareOrder();

    Rest *rest_;

    EntityLayer::UserEntity *user_;
    EntityLayer::OrderEntity *order_;

    ModelLayer::OrderModel *model_;

    int totalToPay_;

};
}

#endif // CARTBUSINESS_H
