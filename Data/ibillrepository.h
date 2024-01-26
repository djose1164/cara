#ifndef IBILLREPOSITORY_H
#define IBILLREPOSITORY_H

namespace DataLayer {
class IBillRepository
{
public:
    virtual ~IBillRepository();

    virtual void fetchByCustomerId(int customerId) = 0;

    virtual void fetchById(int orderId) = 0;

    virtual void fetchByAdminId(int adminId) = 0;
};
}

#endif // IBILLREPOSITORY_H
