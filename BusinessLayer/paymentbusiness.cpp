#include "paymentbusiness.h"

using BusinessLayer::PaymentBusiness;

PaymentBusiness::PaymentBusiness(QObject *parent)
    : QObject{parent}
    , rest_{new Rest(this)}
    , payment_{nullptr}
{
    rest_->base("http://127.0.0.1:5000/api/payments");

    QSettings settings{};
    const QString token{settings.value("Credentials/refreshToken").toString()};

    Q_ASSERT(!token.isEmpty());
    rest_->refreshToken(token);
    rest_->refreshAccessToken();

    connect(rest_, &Rest::sent, this, [this]() {
        emit requested();
    });
}

void PaymentBusiness::setPayment(PaymentEntity * const payment)
{
    if (payment_ != payment)
    {
        payment_ = payment;
        emit paymentChanged();
    }
}

PaymentEntity *PaymentBusiness::payment() const
{
    return payment_;
}

void PaymentBusiness::patch()
{
    Q_ASSERT(payment_->paymentId());

    const auto doc{QJsonDocument::fromJson(payment_->toJson())};

    QVariantMap map{doc.toVariant().toMap()};
    rest_->patch("/", map);

    connect(rest_, &Rest::succeeded, this, [this]() {
        emit patchSucceeded();
    });

    connect(rest_, &Rest::failed, this, [this]() {
        emit patchFailed();
    });
}

void PaymentBusiness::update()
{
    const int amount{payment_->paidAmount()};

    // TODO: Implement this.
}

void PaymentBusiness::setAsPaid()
{
    qDebug() << "PaymentBusiness: setting as paid.";
    payment_->setPaidAmount(payment_->amountToPay());
    patch();
}

void PaymentBusiness::setAsUnpaid()
{
    qDebug() << "PaymentBusiness: setting as unpaid.";
    payment_->setPaidAmount(0);
    patch();
}
