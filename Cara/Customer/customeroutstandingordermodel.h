#ifndef CUSTOMEROUTSTANDINGORDERMODEL_H
#define CUSTOMEROUTSTANDINGORDERMODEL_H

#include "customerrepository.h"
#include <QAbstractListModel>
#include <QQmlEngine>
namespace Customer
{
class CustomerOutstandingOrderModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(CustomerRepository *customerRepository READ customerRepository
                   WRITE setCustomerRepository NOTIFY customerRepositoryChanged FINAL)
    QML_ELEMENT
public:
    enum class RoleNames
    {
        ForenameRole = Qt::UserRole + 1,
        SurnameRole,
        CustomerNameRole,
        CustomerIdRole,
        NumberOfOrdersRole,
        TotalToPayRole
    };

    explicit CustomerOutstandingOrderModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    CustomerRepository *customerRepository() const;
    void setCustomerRepository(CustomerRepository *newCustomerRepository);

public slots:
    void fetchCustomersWithOutstandingOrders();

signals:
    void customerRepositoryChanged();

private:
    void customersFetched(const QList<QJsonObject> &customerList);

    QList<QJsonObject> data_;
    QPointer<CustomerRepository> customerRepository_;
};
}

#endif // CUSTOMEROUTSTANDINGORDERMODEL_H
