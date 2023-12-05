#ifndef ASSOCIATESALESPERSONSERVICE_H
#define ASSOCIATESALESPERSONSERVICE_H

#include "data/salespersonrepository.h"
#include <QObject>
#include <QQmlEngine>

namespace Salesperson {
class AssociateSalespersonService : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QByteArray data READ data WRITE setData NOTIFY dataChanged FINAL)
    Q_PROPERTY(int salespersonId READ salespersonId WRITE setSalespersonId NOTIFY salespersonIdChanged FINAL)
    Q_PROPERTY(int adminId READ adminId WRITE setAdminId NOTIFY adminIdChanged FINAL)

public:
    explicit AssociateSalespersonService(QObject *parent = nullptr);

    QByteArray data() const;
    void setData(const QByteArray &newData);

    int salespersonId() const;
    void setSalespersonId(int newSalespersonId);

    int adminId() const;
    void setAdminId(int newAdminId);

public slots:
    void fetchAllAssociateSalesperson();
    void addAssociateSalesperson(const QVariantMap &data);
    void fetchAssociateSalespersonById(int id);
    void fetchCustomers();

    void patch(const QVariantMap &data);

signals:
    void errorOccurred();
    void fetched();
    void added();
    void patched();

    void dataChanged();

    void salespersonIdChanged();

    void adminIdChanged();

private:
    Data::SalespersonRepository *salespersonRepository_;
    QByteArray data_;

    int salespersonId_;
    int adminId_;
};
}
#endif // ASSOCIATESALESPERSONSERVICE_H
