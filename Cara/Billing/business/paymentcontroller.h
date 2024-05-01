#ifndef PAYMENTCONTROLLER_H
#define PAYMENTCONTROLLER_H

#include "payment.h"
#include <QObject>
#include <QQmlEngine>

namespace Controller
{
class PaymentController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit PaymentController(QObject *parent = nullptr);
    
    Q_INVOKABLE Entity::Payment fromJsonObject(const QJsonObject &data) const;
signals:
};
}

#endif // PAYMENTCONTROLLER_H
