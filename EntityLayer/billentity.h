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
    Q_PROPERTY(QList<DetailsEntity *> details READ details WRITE setDetails NOTIFY detailsChanged)
    Q_PROPERTY(PaymentEntity *payment MEMBER payment_ NOTIFY paymentChanged)

public:
    explicit OrderEntity(QObject *parent = nullptr);

    UserEntity *user() const;

    PaymentEntity *payment() const;
    QList<DetailsEntity *> details() const;

    DetailsEntity *detail(qsizetype index) const;

public slots:
    void appendDetail(DetailsEntity *detail);
    void clear();
    void replaceDetail(qsizetype index, DetailsEntity *detail);
    void popBack();

    void setDetails(const QList<DetailsEntity *> &detail);
    void setUser(UserEntity *const user);

    Q_INVOKABLE QByteArray toJson() const;

signals:
    void userChanged();
    void detailsChanged();
    void paymentChanged();

private:
    UserEntity *user_;
    QList<DetailsEntity *> details_;
    PaymentEntity *payment_;
};
}

#endif // ORDERENTITY_H
