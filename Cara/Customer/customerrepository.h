#ifndef CUSTOMERREPOSITORY_H
#define CUSTOMERREPOSITORY_H

#include <QQmlEngine>

class QRestReply;

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

#endif // CUSTOMERREPOSITORY_H
