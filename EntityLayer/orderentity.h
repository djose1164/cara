#ifndef ORDERENTITY_H
#define ORDERENTITY_H

#include "paymententity.h"
#include "detailsentity.h"
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
    Q_PROPERTY(QList<DetailsEntity *> details READ details WRITE setDetails NOTIFY detailsChanged)
    Q_PROPERTY(PaymentEntity *payment MEMBER payment_ NOTIFY paymentChanged)

public:
    explicit OrderEntity(QObject *parent = nullptr);

    int customerId() const;
    int userId() const;

    QDateTime date() const;

    PaymentEntity *payment() const;
    QList<DetailsEntity *> details() const;

    DetailsEntity *detail(qsizetype index) const;

public slots:
    void appendDetail(DetailsEntity *detail);
    void clear();
    void replaceDetail(qsizetype index, DetailsEntity *detail);
    void popBack();

    void setDetails(const QList<DetailsEntity *> &detail);

signals:
    void customerIdChanged();
    void userIdChanged();
    void dateChanged();
    void detailsChanged();
    void paymentChanged();

private:
    int customerId_;
    int userId_;
    QDateTime date_;
    QList<DetailsEntity *> details_;
    PaymentEntity *payment_;
};
}

#endif // ORDERENTITY_H
