#ifndef PAY_H
#define PAY_H

#include <QNetworkRequestFactory>
#include <QQmlEngine>
#include "Utils/abstractresource.h"

namespace Business
{
class Pay : public AbstractResource
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged FINAL)
    Q_PROPERTY(int paymentId READ paymentId WRITE setPaymentId NOTIFY paymentIdChanged FINAL)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged FINAL)
public:
    explicit Pay(QObject *parent = nullptr);

    QString errorString() const;
    void setErrorString(const QString &newErrorString);

    int paymentId() const;
    void setPaymentId(int newPaymentId);

    QString path() const;
    void setPath(const QString &newPath);

public slots:
    void pay(const QVariantMap &data);
    void pay(int amount);

private slots:
    void paymentFinished(QRestReply *reply);

signals:
    void paid(const QJsonObject &payment);
    void errorOccurred();

    void errorStringChanged();

    void paymentIdChanged();

    void pathChanged();

private:
    QString errorString_;
    int paymentId_;
    QString path_;
};
}

Q_DECLARE_OPAQUE_POINTER(QRestReply *)

#endif // PAY_H
