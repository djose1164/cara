#ifndef CUSTOMERMODEL_H
#define CUSTOMERMODEL_H

#include "customerentity.h"
#include "rest.h"
#include <QAbstractListModel>
#include <QtQml>

namespace ModelLayer {
using EntityLayer::CustomerEntity;

class CustomerModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum class RoleNames
    {
        ForenameRole = Qt::UserRole,
        SurnameRole,
        BillsRole,
        CustomerIdRole,
        TypeRole,
    };

    explicit CustomerModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    QHash<int, QByteArray> roleNames() const override;

    int count() const;

public slots:
    void populate();
    void fetchAllCustomers();
    void fetchCustomerById(int customerId);

    void append(CustomerEntity *const customer);
    void insert(int index, CustomerEntity *const customer);
    void clear();

signals:
    void allCustomersFetched(const QVariantMap &resp);
    void populated();

private:
    QList<CustomerEntity *> data_;
    QHash<int, QByteArray> roleNames_;

    Rest *rest_;
};
}

#endif // CUSTOMERMODEL_H
