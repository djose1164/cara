#ifndef RESTCUSTOMERREPOSITORY_H
#define RESTCUSTOMERREPOSITORY_H

#include "customerrepository.h"
#include <QQmlEngine>
#include <QNetworkRequestFactory>

class QRestAccessManager;
class RestCustomerRepository : public Customer::CustomerRepository
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit RestCustomerRepository(QObject *parent = nullptr);

    // CustomerRepository interface
    void readBySalespersonId(SalespersonId salespersonId) override;
    void fetchCustomersWithOutstandingOrders() override;

private:
    QRestAccessManager *manager_;
    std::shared_ptr<QNetworkRequestFactory> api_;
    const QString path_;
};

#endif // RESTCUSTOMERREPOSITORY_H
