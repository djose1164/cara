#include "pay.h"
#include "payment.h"
#include <QRestReply>
#include <QSettings>
#include <QJsonObject>
#include <QJsonDocument>
#include <QRestAccessManager>

namespace Business
{
Pay::Pay(QObject *parent)
    : AbstractResource{parent}
    , errorString_{}
    , paymentId_{}
    , path_{}
{
}

void Pay::pay(const QVariantMap &data)
{
    const auto p{Entity::Payment::fromMap(data)};
    if (!p.isPayeable())
    {
        const QString error{"Pay::pay: data is invalid."};
        qDebug() << error << data;
        setErrorString(error);

        emit errorOccurred();
        return;
    }

    if (manager_ == nullptr)
    {
        qCritical() << "Pay::pay: manager_ can't be null. ";
        return;
    }

    manager_->post(api_->createRequest(path_ + QString("/%1").arg(p.paymentId())),
                   QJsonDocument{p.toJsonObject()},
                   this,
                   [&](QRestReply &r) { paymentFinished(&r); });
}

void Pay::pay(int amount)
{
    Entity::Payment p{};
    p.setPaymentId(paymentId_);
    p.setAmount(amount);

    if (!p.isPayeable())
    {
        const QString error{"Pay::pay: Entity::Payment p is invalid."};
        setErrorString(error);

        emit errorOccurred();
        return;
    }

    if (manager_ == nullptr)
    {
        qCritical() << "Pay::pay: manager_ can't be null. ";
        return;
    }

    manager_->post(api_->createRequest(path_ + QString("/%1").arg(p.paymentId())),
                   QJsonDocument{p.toJsonObject()},
                   this,
                   [&](QRestReply &r) { paymentFinished(&r); });
}

void Pay::paymentFinished(QRestReply *reply)
{
    if (!reply->isSuccess())
    {
        setErrorString(reply->errorString());
        emit errorOccurred();
        return;
    }

    const auto json{reply->readJson()};
    if (!json)
    {
        qWarning() << "Pay::pay: json not valid.";
        emit paid({});
        return;
    }
    qDebug() << "Pay::pay: payment was successful.";
    emit paid(json.value().object());
}

QString Pay::path() const
{
    return path_;
}

void Pay::setPath(const QString &newPath)
{
    if (path_ == newPath)
        return;
    path_ = newPath;
    emit pathChanged();
}

int Pay::paymentId() const
{
    return paymentId_;
}

void Pay::setPaymentId(int newPaymentId)
{
    if (paymentId_ == newPaymentId)
        return;
    paymentId_ = newPaymentId;
    emit paymentIdChanged();
}

QString Pay::errorString() const
{
    return errorString_;
}

void Pay::setErrorString(const QString &newErrorString)
{
    if (errorString_ == newErrorString)
        return;
    errorString_ = newErrorString;
    emit errorStringChanged();
}
}
