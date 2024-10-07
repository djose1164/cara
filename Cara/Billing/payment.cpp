#include "payment.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace Entity
{
Payment Payment::fromMap(const QVariantMap &map)
{
    Payment p{};
    p.setPaymentId(map.value("paymentId").toInt());
    p.setOrderId(map.value("orderId").toInt());
    p.setAmountToPay(map.value("amountToPay").toInt());
    p.setPaidAmount(map.value("paidAmount").toInt());
    p.setAmount(map.value("amount").toInt());

    return p;
}

Entity::Payment Entity::Payment::fromJsonObject(const QJsonObject &json)
{
    Payment payment{};
    payment.setAmountToPay(json["amount_to_pay"].toInt());
    payment.setPaidAmount(json["paid_amount"].toInt());
    payment.setPaymentId(json["id"].toInt());
    payment.setStatus(static_cast<Status>(json["payment_status_id"].toInt()));
    const QString updatedAtString{json["updated_at"].toString()};
    payment.setUpdatedAt(QDateTime::fromString(updatedAtString, Qt::ISODate));

    return payment;
}

Payment::Payment()
    : paymentId_{}
    , paidAmount_{}
    , amountToPay_{}
    , orderId_{}
    , amount_{}
    , status_{Status::Any}
{
}

bool Payment::operator==(const Payment &other) const
{
    return paymentId_ == other.paymentId_
           && orderId_ == other.orderId_
           && paidAmount_ == other.paidAmount_
           && amountToPay_ == other.amountToPay_
           && status_ == other.status_;
}

int Payment::paymentId() const
{
    return paymentId_;
}

void Payment::setPaymentId(int newPaymentId)
{
    paymentId_ = newPaymentId;
}

int Payment::paidAmount() const
{
    return paidAmount_;
}

void Payment::setPaidAmount(int newPaidAmount)
{
    paidAmount_ = newPaidAmount;
}

int Payment::amountToPay() const
{
    return amountToPay_;
}

void Payment::setAmountToPay(int newAmountToPay)
{
    amountToPay_ = newAmountToPay;
}

int Payment::orderId() const
{
    return orderId_;
}

void Payment::setOrderId(int newOrderId)
{
    orderId_ = newOrderId;
}

bool Payment::isValid() const
{
    return amountToPay_ >= paidAmount_
           && orderId_ > 0
           && paymentId_ > 0;
}

bool Payment::isPayeable() const
{
    return paymentId_ > 0 && amount_ > 0;
}

int Payment::amount() const
{
    return amount_;
}

void Payment::setAmount(int newAmount)
{
    amount_ = newAmount;
}

QJsonObject Payment::toJsonObject() const
{
    QJsonObject json{};
    json["payment_id"] = paymentId_;
    json["amount"] = amount_;
    return json;
}

QByteArray Payment::toJson() const
{
    if (status_ == Status::Any)
        throw std::runtime_error{"Payment::toJson: invalid status."};

    QJsonObject object{};
    object["paid_amount"] = paidAmount_;
    object["amount_to_pay"] = amountToPay_;

    const int statusInt{int(status())};
    object["payment_status_id"] = statusInt;

    return QJsonDocument{object}.toJson(QJsonDocument::Compact);
}

Payment::Status Payment::status() const
{
    return status_;
}

void Payment::setStatus(Status newStatus)
{
    status_ = newStatus;
}

QString Payment::statusString() const
{
    switch (status_)
    {
        using enum Status;
    case Paid:
        return "Pagado";
    case Unpaid:
        return "Credito";
    case Credited:
        return "Abonado";
    default:
        return "Indefinido";
    }
}

QDateTime Payment::updatedAt() const
{
    return updatedAt_;
}

void Payment::setUpdatedAt(const QDateTime &newUpdatedAt)
{
    updatedAt_ = newUpdatedAt;
}

void Entity::Payment::decideStatus()
{
    using enum Status;
    if (amountToPay_ == paidAmount_)
        setStatus(Paid);
    else if (paidAmount_ == 0)
        setStatus(Unpaid);
    else if (paidAmount_ > 0 && paidAmount_ < amountToPay_)
        setStatus(Credited);
    else
        setStatus(Any);
}
}
