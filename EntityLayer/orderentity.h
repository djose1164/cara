#ifndef ORDERENTITY_H
#define ORDERENTITY_H

#include "paymententity.h"
#include "productentity.h"
#include <QObject>
#include <QtQml>

namespace EntityLayer {
class OrderEntity : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int customerId MEMBER customerId_ NOTIFY customerIdChanged)
    Q_PROPERTY(int userId MEMBER userId_ NOTIFY userIdChanged)
    Q_PROPERTY(QDateTime date MEMBER date_ NOTIFY dateChanged)
    Q_PROPERTY(QQmlListProperty<ProductEntity> products READ products NOTIFY detailsChanged)
    Q_PROPERTY(PaymentEntity *payment MEMBER payment_ NOTIFY paymentChanged)

public:
    explicit OrderEntity(QObject *parent = nullptr);
    QQmlListProperty<ProductEntity> products();

    qsizetype count() const;
    ProductEntity *product(qsizetype index) const;

public slots:
    void appendProduct(ProductEntity *product);
    void clear();
    void replaceProduct(qsizetype index, ProductEntity *detail);
    void popBack();

signals:
    void customerIdChanged();
    void userIdChanged();
    void dateChanged();
    void detailsChanged();
    void paymentChanged();

private:
    static void append(QQmlListProperty<ProductEntity> *list, ProductEntity *detail);
    static qsizetype count(QQmlListProperty<ProductEntity> *list);
    static void clear(QQmlListProperty<ProductEntity> *list);
    static void replace(QQmlListProperty<ProductEntity> *list, qsizetype index, ProductEntity *detail);
    static void popBack(QQmlListProperty<ProductEntity> *list);
    static ProductEntity *product(QQmlListProperty<ProductEntity> *list, qsizetype index);

    int customerId_;
    int userId_;
    QDateTime date_;
    QList<ProductEntity *> products_;
    PaymentEntity *payment_;
};
}

#endif // ORDERENTITY_H
