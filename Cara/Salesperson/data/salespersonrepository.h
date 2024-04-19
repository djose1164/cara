#ifndef SALESPERSONREPOSITORY_H
#define SALESPERSONREPOSITORY_H

#include <QByteArray>

namespace Entity { class Salesperson; }
class QNetworkReply;

namespace Data {
class SalespersonRepository
{
public:
    using AdminId = int;
    using OrderId = int;
    using SalespersonId = int;

    virtual ~SalespersonRepository();
    
    virtual void getSalespersonById(int id) = 0;

    virtual void fetchSalesperson(AdminId adminId) = 0;

    virtual void addAssociateSalesperson(const Entity::Salesperson &salesperson) = 0;

    virtual void patch(int id, const QByteArray &payload) = 0;

    virtual void getCustomers(int id) = 0;

    virtual void remove(int id) = 0;

    [[nodiscard]] virtual QNetworkReply *takeOrder(OrderId orderId, SalespersonId salespersonId) = 0;

    [[nodiscard]] virtual QNetworkReply *abandonOrder(OrderId orderId) = 0;

    [[nodiscard]] virtual QNetworkReply *markOrderAsDone(OrderId orderId) = 0;

    [[nodiscard]] virtual QNetworkReply *fetchSalespeopleByAdminId(AdminId adminId) = 0;

    [[nodiscard]] virtual QNetworkReply *canOrderBePaid(SalespersonId salespersonId, OrderId orderId) = 0;
};
}

#endif // SALESPERSONREPOSITORY_H
