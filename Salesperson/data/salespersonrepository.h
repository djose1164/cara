#ifndef SALESPERSONREPOSITORY_H
#define SALESPERSONREPOSITORY_H

#include "salesperson.h"

namespace Data {
class SalespersonRepository
{
public:
    virtual ~SalespersonRepository();
    
    virtual void getSalespersonById(int id) = 0;

    virtual void getAssociateSalespersons() = 0;

    virtual void addAssociateSalesperson(const Entity::Salesperson &salesperson) = 0;

    virtual void patch(int id, const QByteArray &payload) = 0;

    virtual void getCustomers(int id) = 0;
};
}

#endif // SALESPERSONREPOSITORY_H
