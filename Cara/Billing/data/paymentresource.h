#ifndef PAYMENTRESOURCE_H
#define PAYMENTRESOURCE_H

#include <QJsonObject>
#include <QQmlEngine>
#include "abstractresource.h"

namespace Data
{
class PaymentResource : public AbstractResource
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QList<QJsonObject> data READ data NOTIFY dataChanged FINAL)
    Q_PROPERTY(QString path MEMBER path_ REQUIRED FINAL)
    Q_PROPERTY(int paymentId MEMBER paymentId_ REQUIRED FINAL)
public:
    explicit PaymentResource(QObject *parent = nullptr);

    QList<QJsonObject> data() const;

public slots:
    void refresh();

signals:
    void dataChanged();

private:
    QList<QJsonObject> data_;
    QString path_;
    int paymentId_;
};
}

#endif // PAYMENTRESOURCE_H
