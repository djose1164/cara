#ifndef SALESPERSONCONTROLLER_H
#define SALESPERSONCONTROLLER_H

#include <QJsonObject>
#include <QObject>
#include <QQmlEngine>
#include "data/salespersonrepository.h"
#include "salesperson.h"

namespace Salesperson {
class SalespersonController : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QByteArray data READ data WRITE setData NOTIFY dataChanged FINAL)
    Q_PROPERTY(int salespersonId READ salespersonId WRITE setSalespersonId NOTIFY salespersonIdChanged FINAL)
    Q_PROPERTY(int adminId READ adminId WRITE setAdminId NOTIFY adminIdChanged FINAL)
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged FINAL)
public:
    using AdminId = Data::SalespersonRepository::AdminId;
    using OrderId = Data::SalespersonRepository::OrderId;

    explicit SalespersonController(QObject *parent = nullptr);

    QByteArray data() const;
    void setData(const QByteArray &newData);

    int salespersonId() const;
    void setSalespersonId(int newSalespersonId);

    int adminId() const;
    void setAdminId(int newAdminId);

    Q_INVOKABLE Entity::Salesperson fromJsonObject(const QJsonObject &data) const;

    QString errorString() const;
    void setErrorString(const QString &newErrorString);

public slots:
    void fetchAllAssociateSalesperson(AdminId adminId);
    void addAssociateSalesperson(const QVariantMap &data);
    void fetchAssociateSalespersonById(int id);
    void fetchCustomers();

    void patch(const QVariantMap &data);

    void takeOrder(OrderId orderId);

    void abandonOrder(OrderId orderId);

    void markOrderAsDone(OrderId orderId);

signals:
    void errorOccurred();
    void fetched();
    void added();
    void patched();

    void dataChanged();

    void salespersonIdChanged();

    void adminIdChanged();

    void errorStringChanged();

    void orderTook();

    void orderAbandoned();

    void orderMarkedAsDone();

private:
    Data::SalespersonRepository *salespersonRepository_;
    QByteArray data_;

    int salespersonId_;
    int adminId_;
    QString errorString_;
};
}
#endif // SALESPERSONCONTROLLER_H
