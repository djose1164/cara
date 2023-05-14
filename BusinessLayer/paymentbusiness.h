#ifndef PAYMENTBUSINESS_H
#define PAYMENTBUSINESS_H

#include "paymententity.h"
#include "rest.h"
#include <QObject>
#include <QtQml>

using EntityLayer::PaymentEntity;

namespace BusinessLayer {
class PaymentBusiness : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(PaymentEntity *payment READ payment WRITE setPayment NOTIFY paymentChanged)

public:
    explicit PaymentBusiness(QObject *parent = nullptr);

    void setPayment(PaymentEntity *const payment);
    PaymentEntity *payment() const;

public slots:
    void patch();
    void update();

    void setAsPaid();
    void setAsUnpaid();

signals:
    void paymentChanged();

    void patchSucceeded();
    void updateSucceeded();
    void patchFailed();
    void updateFailed();
    void requested();

private:
    Rest *rest_;
    PaymentEntity *payment_;
};
}

#endif // PAYMENTBUSINESS_H
