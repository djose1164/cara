#ifndef CUSTOMERREPOSITORY_H
#define CUSTOMERREPOSITORY_H

#include <QQmlEngine>

class QRestReply;

namespace Customer
{
class CustomerRepository : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("CustomerRepository is an abstract class!")
public:
    using SalespersonId = int;
    using QObject::QObject;

    virtual ~CustomerRepository();

    virtual void readBySalespersonId(SalespersonId salespersonId) = 0;

    // Fetch every customer with outstanding orders--payment_status_id is 3 or 2--
    // and retrieve a brief summary.
    virtual void fetchCustomersWithOutstandingOrders() = 0;

signals:
    void errorOccurred(int error, const QString &errorString);

    void fetched(const QList<QJsonObject> &data);
};
}

#endif // CUSTOMERREPOSITORY_H
