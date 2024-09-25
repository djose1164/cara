#ifndef CARTCONTROLLER_H
#define CARTCONTROLLER_H

#include "userentity.h"
#include "orderentity.h"
#include "model/ordermodel.h"
#include "abstractresource.h"
#include "productentity.h"

#include <QObject>
#include <QQmlEngine>

namespace Shopping {
class CartController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(EntityLayer::OrderEntity *order READ order WRITE setOrder NOTIFY orderChanged)
    Q_PROPERTY(Model::OrderModel *model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(int totalToPay READ totalToPay NOTIFY totalToPayChanged)
    Q_PROPERTY(QList<int> cartedProductIds READ cartedProductIds NOTIFY cartedProductIdsChanged FINAL)

public:
    explicit CartController(QObject *parent = nullptr);

    EntityLayer::OrderEntity *order() const;
    Model::OrderModel *model() const;
    
    Q_INVOKABLE bool isProductInCart(const EntityLayer::Product &product) const;
    Q_INVOKABLE int totalToPay() const;

    QList<int> cartedProductIds() const;
    void setCartedProductIds(const QList<int> &newCartedProductIds);

public slots:
    void setOrder(EntityLayer::OrderEntity *const order);
    void setModel(Model::OrderModel *const model);

    void addtoCart(const EntityLayer::Product &detail);
    void removeFromCart(const int index);

    /**
     * @brief checkout Create an order and send it to the server. This method
     * do some checking process.
     */
    void checkout(const Entity::Customer &buyer);


signals:
    void modelChanged();
    void orderChanged();
    void checkoutSucceeded();
    void checkoutFailed(const QVariantMap &resp);
    void totalToPayChanged();
    void errorOccurred(const QString &errorString);

    void cartedProductIdsChanged();

private:
    void prepareOrder(const Entity::Customer &buyer);
    void appendProductId(int productId);
    void removeProductId(int productId);
    
    AbstractResource::Api api_;
    AbstractResource::Manager manager_;
    EntityLayer::OrderEntity *order_;
    Model::OrderModel *model_;

    int totalToPay_;
    QList<int> cartedProductIds_; // Holds the product's ids stored in the cart.
};
}

#endif // CARTCONTROLLER_H
