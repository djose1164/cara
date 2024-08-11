#ifndef ENTITY_CUSTOMER_H
#define ENTITY_CUSTOMER_H

#include <QObject>
#include <QQmlEngine>

using CustomerId = int;

namespace Entity
{
class Customer
{
    Q_GADGET
    QML_VALUE_TYPE(customer)
    QML_STRUCTURED_VALUE
    Q_PROPERTY(QString forename READ forename WRITE setForename FINAL)
    Q_PROPERTY(QString surname READ surname WRITE setSurname FINAL)
    Q_PROPERTY(QString phoneNumber READ phoneNumber WRITE setPhoneNumber FINAL)
    Q_PROPERTY(int customerId READ customerId WRITE setCustomerId FINAL)
    Q_PROPERTY(bool valid READ valid FINAL)
public:
    static Customer fromJson(const QJsonObject &customerObject);

    Customer();

    bool operator==(const Customer &rhs) const;
    bool operator!=(const Customer &rhs) const;

    QString forename() const;
    void setForename(const QString &newForename);

    QString surname() const;
    void setSurname(const QString &newSurname);

    QString phoneNumber() const;
    void setPhoneNumber(const QString &newPhoneNumber);

    CustomerId customerId() const;
    void setCustomerId(CustomerId newCustomerId);

    bool valid() const;

private:
    QString forename_;
    QString surname_;
    QString phoneNumber_;
    CustomerId customerId_;
};
}

#endif // ENTITY_CUSTOMER_H
