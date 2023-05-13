#ifndef PAYMENTENTITY_H
#define PAYMENTENTITY_H

#include "jsonexportable.h"
#include <QObject>
#include <QtQml>

namespace EntityLayer {
class PaymentEntity : public QObject, public JsonExportable
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int paymentId READ paymentId WRITE setPaymentId NOTIFY paymentIdChanged)
    Q_PROPERTY(int amountToPay READ amountToPay WRITE setAmountToPay NOTIFY amountToPayChanged)
    Q_PROPERTY(int paidAmount READ paidAmount WRITE setPaidAmount NOTIFY paidAmoutChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)

public:
    enum class Status
    {
        Paid,
        Unpaid,
        Credited
    };
    Q_ENUM(Status)

    explicit PaymentEntity(QObject *parent = nullptr);

    void setPaymentId(int id);
    int paymentId() const;

    void setPaidAmount(int amount);
    int paidAmount() const;

    void setAmountToPay(int amount);
    int amountToPay() const;

    Status status() const;

    Q_INVOKABLE QByteArray toJson() const override;

signals:
    void paymentIdChanged();
    void amountToPayChanged();
    void paidAmoutChanged();
    void statusChanged();

private:
    int paymentId_;
    int paidAmount_;
    int amountToPay_;
};
}

#endif // PAYMENTENTITY_H
