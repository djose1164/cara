#ifndef ORDERENTITY_H
#define ORDERENTITY_H

#include <QObject>
#include <QtQml>
#include "../Customer/entity/customer.h"
#include "detailsentity.h"
#include "payment.h"

namespace EntityLayer {
class OrderEntity : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(Entity::Customer customer READ customer WRITE setCustomer NOTIFY customerChanged FINAL)
    Q_PROPERTY(Entity::Payment payment MEMBER payment_ NOTIFY paymentChanged FINAL)

public:
    explicit OrderEntity(QObject *parent = nullptr);
    
    Entity::Customer &customer();

    Entity::Payment payment() const;

    QByteArray details() const;
    void setDetails(const QByteArray &detail);
    QBindable<QByteArray> bindableDetails();

    DetailsEntity *detail(qsizetype index) const;

public slots:
    void setCustomer(const Entity::Customer &user);

    Q_INVOKABLE QByteArray toJson();

signals:
    void customerChanged();
    void detailsChanged();
    void paymentChanged();

private:
    Entity::Customer customer_;
    QProperty<QByteArray> details_;
    Entity::Payment payment_;
};
}

#endif // ORDERENTITY_H
