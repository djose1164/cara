#ifndef PAY_H
#define PAY_H

#include <QNetworkRequestFactory>
#include <QQmlEngine>
#include "abstractresource.h"

namespace Business
{
class Pay : public AbstractResource
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged FINAL)
    Q_PROPERTY(int paymentId READ paymentId WRITE setPaymentId NOTIFY paymentIdChanged FINAL)

public:
    explicit Pay(QObject *parent = nullptr);

    QString errorString() const;
    void setErrorString(const QString &newErrorString);

    int paymentId() const;
    void setPaymentId(int newPaymentId);

public slots:
    void pay(const QVariantMap &data);
    void pay(int amount);

signals:
    void paid(const QJsonObject &payment);
    void errorOccurred();

    void errorStringChanged();

    void paymentIdChanged();

private:
    QString errorString_;
    int paymentId_;
};
}

#endif // PAY_H
