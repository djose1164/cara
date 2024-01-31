#ifndef CUSTOMERREPOSITORY_H
#define CUSTOMERREPOSITORY_H

#include "salesperson.h"
#include <QQmlEngine>

namespace Business
{
class CustomerRepository
{
public:
    using SalespersonId = Entity::Salesperson::SalespersonId;

    virtual ~CustomerRepository();

    virtual void readBySalespersonId(SalespersonId salespersonId) = 0;
};
}
Q_DECLARE_INTERFACE(Business::CustomerRepository, "com.cara.CustomerRepository")

#endif // CUSTOMERREPOSITORY_H
