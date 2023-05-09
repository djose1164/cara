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

    Q_PROPERTY(int amountToPay MEMBER amountToPay_ NOTIFY amountToPayChanged)
    Q_PROPERTY(int paidAmount MEMBER paidAmount_ NOTIFY paidAmoutChanged)

public:
    explicit PaymentEntity(QObject *parent = nullptr);

    Q_INVOKABLE QByteArray toJson() const override;

signals:
    void amountToPayChanged();
    void paidAmoutChanged();

private:
    int paidAmount_;
    int amountToPay_;
};
}

#endif // PAYMENTENTITY_H
