#include "customeroutstandingordermodel.h"
#include <QJsonObject>

using namespace Qt::StringLiterals;

namespace Customer
{
CustomerOutstandingOrderModel::CustomerOutstandingOrderModel(QObject *parent)
    : QAbstractListModel{parent}
    , data_{}
    , customerRepository_{}
{
}

int CustomerOutstandingOrderModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return data_.count();
}

QVariant CustomerOutstandingOrderModel::data(const QModelIndex &index, int role) const
{
    if (not index.isValid())
        return {};

    const auto &entry{data_.at(index.row())};
    switch (static_cast<RoleNames>(role))
    {
    case RoleNames::CustomerNameRole:
        return entry["customer_name"_L1].toString();
    case RoleNames::NumberOfOrdersRole:
        return entry["number_of_orders"_L1].toInt();
    case RoleNames::TotalToPayRole:
        return entry["total_to_pay"_L1].toInt();
    case RoleNames::ForenameRole:
        return entry["forename"_L1].toString();
    case RoleNames::SurnameRole:
        return entry["surname"_L1].toString();;
    case RoleNames::CustomerIdRole:
        return entry["customer_id"_L1].toInt();
    }

    return {};
}

QHash<int, QByteArray> CustomerOutstandingOrderModel::roleNames() const
{
    return {{int(RoleNames::CustomerNameRole), "customerName"_ba},
            {int(RoleNames::NumberOfOrdersRole), "numberOfOrders"_ba},
            {int(RoleNames::TotalToPayRole), "totalToPay"_ba},
            {int(RoleNames::ForenameRole), "forename"_ba},
            {int(RoleNames::SurnameRole), "surname"_ba},
            {int(RoleNames::CustomerIdRole), "customerId"_ba},};
}

CustomerRepository *CustomerOutstandingOrderModel::customerRepository() const
{
    return customerRepository_;
}

void CustomerOutstandingOrderModel::setCustomerRepository(CustomerRepository *newCustomerRepository)
{
    if (customerRepository_ == newCustomerRepository)
        return;

    if (customerRepository_)
        customerRepository_->disconnect(this);

    customerRepository_ = newCustomerRepository;

    if (customerRepository_)
    {
        connect(customerRepository_,
                &CustomerRepository::fetched,
                this,
                &CustomerOutstandingOrderModel::customersFetched);
    }

    emit customerRepositoryChanged();
}

void CustomerOutstandingOrderModel::fetchCustomersWithOutstandingOrders()
{
    if (customerRepository_)
    {
        qDebug() << "CustomerOutstandingOrderModel::fetchCustomersWithOutstandingOrders: fetching...";
        customerRepository_->fetchCustomersWithOutstandingOrders();
    }
}

void CustomerOutstandingOrderModel::customersFetched(const QList<QJsonObject> &customerList)
{
    Q_ASSERT(customerList.size() > 0);
    beginResetModel();
    data_.clear();
    data_ = customerList;
    endResetModel();
}
}
