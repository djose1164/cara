#ifndef ASSOCIATESALESPERSONREPOSITORY_H
#define ASSOCIATESALESPERSONREPOSITORY_H

#include "entity/salesperson.h"

namespace Data {
class AssociateSalespersonRepository
{
public:
    virtual ~AssociateSalespersonRepository();

    virtual void getAssociateSalespersonById(int id) = 0;

    virtual void getAssociateSalespersons() = 0;

    virtual void addAssociateSalesperson(const Entity::Salesperson &salesperson) = 0;
};
}

#endif // ASSOCIATESALESPERSONREPOSITORY_H
