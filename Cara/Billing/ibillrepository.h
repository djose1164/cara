#ifndef IBILLREPOSITORY_H
#define IBILLREPOSITORY_H

class QNetworkReply;

namespace DataLayer {
class IBillRepository
{
public:
    using SalespersonId = int;

    virtual ~IBillRepository();

    virtual void fetchByCustomerId(int customerId) = 0;

    virtual void fetchById(int orderId) = 0;

    virtual void fetchByAdminId(int adminId) = 0;

    [[nodiscard]] virtual QNetworkReply *fetchUnpaidBills() = 0;

    [[nodiscard]] virtual QNetworkReply *fetchOrdersInQueue(SalespersonId spId = 0) = 0;
};
}

#endif // IBILLREPOSITORY_H
