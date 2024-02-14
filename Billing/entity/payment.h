#ifndef PAYMENT_H
#define PAYMENT_H

#include <QObject>
#include <QQmlEngine>

namespace Entity
{
class Payment
{
    Q_GADGET
    QML_VALUE_TYPE(payment)
    Q_PROPERTY(int paymentId READ paymentId WRITE setPaymentId FINAL)
    Q_PROPERTY(int paidAmount READ paidAmount WRITE setPaidAmount FINAL)
    Q_PROPERTY(int amountToPay READ amountToPay WRITE setAmountToPay FINAL)
    Q_PROPERTY(int orderId READ orderId WRITE setOrderId FINAL)
    Q_PROPERTY(int amount READ amount WRITE setAmount FINAL)
    Q_PROPERTY(Status status READ status WRITE setStatus FINAL)
    Q_PROPERTY(QString statusString READ statusString FINAL)
    Q_PROPERTY(QDateTime updatedAt READ updatedAt WRITE setUpdatedAt FINAL)

public:
    enum class Status
    {
        Paid = 1,
        Unpaid,
        Credited,
        Any
    };
    Q_ENUM(Status)

    static Payment fromMap(const QVariantMap &map);
    static Payment fromJsonObject(const QJsonObject &json);

    Payment();

    int paymentId() const;
    void setPaymentId(int newPaymentId);

    int paidAmount() const;
    void setPaidAmount(int newPaidAmount);

    int amountToPay() const;
    void setAmountToPay(int newAmountToPay);

    int orderId() const;
    void setOrderId(int newOrderId);

    Q_INVOKABLE bool isValid() const;

    /**
     * @brief This should be call before calling `pay()`. Checks whether the
     * entity is suitable to process a pay.
     * @return true if the entity valid for paying; false otherwise.
     */
    Q_INVOKABLE bool isPayeable() const;

    int amount() const;
    void setAmount(int newAmount);

    QJsonObject toJsonObject() const;
    QByteArray toJson() const;

    Status status() const;
    void setStatus(Status newStatus);

    QString statusString() const;

    QDateTime updatedAt() const;
    void setUpdatedAt(const QDateTime &newUpdatedAt);

private:
    int paymentId_;
    int paidAmount_;
    int amountToPay_;
    int orderId_;
    int amount_;
    Status status_;
    QDateTime updatedAt_;
};

class PaymentDerived : public Payment
{
    Q_GADGET
};
}

namespace PaymentDerivedForeign
{
Q_NAMESPACE
QML_NAMED_ELEMENT(Payment)
QML_FOREIGN_NAMESPACE(Entity::PaymentDerived)
}

#endif // PAYMENT_H
