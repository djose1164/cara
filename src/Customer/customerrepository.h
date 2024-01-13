#ifndef CUSTOMERREPOSITORY_H
#define CUSTOMERREPOSITORY_H

#include "salesperson/salesperson.h"
#include <QQmlEngine>

namespace Business
{
class CustomerRepository : public QObject
{
    Q_OBJECT
    QML_INTERFACE
public:
    using SalespersonId = Entity::Salesperson::SalespersonId;
    using QObject::QObject;

    virtual ~CustomerRepository();

    virtual void readBySalespersonId(SalespersonId salespersonId) = 0;

signals:
    void errorOccurred();
    void fetched(const QByteArray &data);
};
}

Q_DECLARE_INTERFACE(Business::CustomerRepository, "com.cara.CustomerRepository")

#endif // CUSTOMERREPOSITORY_H
