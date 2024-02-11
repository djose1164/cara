#ifndef CUSTOMERREPOSITORY_H
#define CUSTOMERREPOSITORY_H

#include <QQmlEngine>

namespace Business
{
class CustomerRepository
{
public:
    using SalespersonId = int;

    virtual ~CustomerRepository();

    virtual void readBySalespersonId(SalespersonId salespersonId) = 0;
};
}
Q_DECLARE_INTERFACE(Business::CustomerRepository, "com.cara.CustomerRepository")

#endif // CUSTOMERREPOSITORY_H
