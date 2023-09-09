#ifndef ORDERENTITY_H
#define ORDERENTITY_H

#include "paymententity.h"
#include "detailsentity.h"
#include "userentity.h"
#include <QObject>
#include <QtQml>

namespace EntityLayer {
class OrderEntity : public QObject, public JsonExportable
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(UserEntity *user READ user WRITE setUser NOTIFY userChanged)
    Q_PROPERTY(PaymentEntity *payment MEMBER payment_ NOTIFY paymentChanged)

public:
    explicit OrderEntity(QObject *parent = nullptr);

    UserEntity *user() const;

    PaymentEntity *payment() const;

    QByteArray details() const;
    void setDetails(const QByteArray &detail);
    QBindable<QByteArray> bindableDetails();

    DetailsEntity *detail(qsizetype index) const;

    int inventoryId() const;
    void setInventoryId(int newInventoryId);

public slots:
    void setUser(UserEntity *const user);

    Q_INVOKABLE QByteArray toJson() const;

signals:
    void userChanged();
    void detailsChanged();
    void paymentChanged();

    void inventoryIdChanged();

private:
    UserEntity *user_;
    QProperty<QByteArray> details_;
    PaymentEntity *payment_;
};
}

#endif // ORDERENTITY_H
