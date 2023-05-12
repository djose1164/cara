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

    Q_PROPERTY(int amountToPay READ amountToPay WRITE setAmountToPay NOTIFY amountToPayChanged)
    Q_PROPERTY(int paidAmount READ paidAmount WRITE setPaidAmount NOTIFY paidAmoutChanged)
    Q_PROPERTY(bool isPaid READ isPaid NOTIFY isPaidChanged)

public:
    explicit PaymentEntity(QObject *parent = nullptr);

    void setPaidAmount(int amount);
    int paidAmount() const;

    void setAmountToPay(int amount);
    int amountToPay() const;

    bool isPaid() const;

    Q_INVOKABLE QByteArray toJson() const override;

signals:
    void amountToPayChanged();
    void paidAmoutChanged();
    void isPaidChanged();

private:
    int paidAmount_;
    int amountToPay_;
};
}

#endif // PAYMENTENTITY_H
