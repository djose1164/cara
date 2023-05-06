#ifndef PAYMENTENTITY_H
#define PAYMENTENTITY_H

#include <QObject>
#include <QtQml>

namespace EntityLayer {
class PaymentEntity : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int amountToPay MEMBER amountToPay_ NOTIFY amountToPayChanged)
    Q_PROPERTY(int paidAmount MEMBER paidAmount_ NOTIFY paidAmoutChanged)

public:
    explicit PaymentEntity(QObject *parent = nullptr);

signals:
    void amountToPayChanged();
    void paidAmoutChanged();

private:
    int paidAmount_;
    int amountToPay_;
};
}

#endif // PAYMENTENTITY_H
