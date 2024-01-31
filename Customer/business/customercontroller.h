#ifndef CUSTOMERCONTROLLER_H
#define CUSTOMERCONTROLLER_H

#include "customer.h"
#include <QObject>
#include <QQmlEngine>

namespace Controller
{
class CustomerController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit CustomerController(QObject *parent = nullptr);

    Q_INVOKABLE Entity::Customer fromMap(const QVariantMap &data) const;

signals:
};
}

#endif // CUSTOMERCONTROLLER_H
