#include "customermodel.h"
#include "currentuser.h"
#include "constants.h"

using ModelLayer::CustomerModel, EntityLayer::BillEntity, EntityLayer::UserEntity, BusinessLayer::Constants;

CustomerModel::CustomerModel(QObject *parent)
    : QAbstractListModel(parent)
    , data_{}
    , rest_{new Rest(this)}
{
    roleNames_[int(RoleNames::BillsRole)] = "bills";
    roleNames_[int(RoleNames::ForenameRole)] = "forename";
    roleNames_[int(RoleNames::SurnameRole)] = "surname";
    roleNames_[int(RoleNames::CustomerIdRole)] = "customerId";
    roleNames_[int(RoleNames::TypeRole)] = "userType";

    const QString base{Constants::instance()->base()};
    rest_->base(base + "/customers");

    const auto user{CurrentUser::instance()};
    connect(user, &CurrentUser::restReady, this, [&](const QString &token) {
        rest_->accessToken(token);
    });
}

int CustomerModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return count();
}

QVariant CustomerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const int row{index.row()};

    switch (static_cast<RoleNames>(role)) {
    case RoleNames::BillsRole:
        return QVariant::fromValue(data_[row]->bills());
    case RoleNames::ForenameRole:
        return data_[row]->forename();
    case RoleNames::SurnameRole:
        return data_[row]->surname();
    case RoleNames::CustomerIdRole:
        return data_[row]->customerId();
    case RoleNames::TypeRole:
        return QVariant::fromValue(data_[row]->userType())
            .toString()
            .toLower();
    default:
        break;
    }

    return {};
}

bool ModelLayer::CustomerModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    const int row{index.row()};

    switch (static_cast<RoleNames>(role)) {
    case RoleNames::BillsRole:
        data_[row]->setBills(value.value<BillModel *>());
        emit dataChanged(index, index, {role});
        return true;
    case RoleNames::ForenameRole:
        data_[row]->setForename(value.toString());
        emit dataChanged(index, index, {role});
        return true;
    case RoleNames::SurnameRole:
        data_[row]->setSurname(value.toString());
        emit dataChanged(index, index, {role});
        return true;
    case RoleNames::CustomerIdRole:
        data_[row]->setCustomerId(value.toInt());
        emit dataChanged(index, index, {role});
        return true;
    default:
        break;
    }

    return false;
}

int CustomerModel::count() const
{
    return data_.count();
}

void CustomerModel::populate()
{
    const auto userInstance{CurrentUser::instance()};
    const auto userType(userInstance->userType());
    using UserTypes = UserEntity::UserTypes;

    Q_ASSERT(userType == UserTypes::ADMIN);

    switch (userType) {
    case UserTypes::ADMIN:
        fetchAllCustomers();
        break;
    case UserTypes::CUSTOMER:
    case UserTypes::USER:
        fetchCustomerById(userInstance->customerId());
        break;
    default:
        qmlInfo(this) << "Type " << QVariant::fromValue(userType).toString()
                      << " is not supported.";
        break;
    }


    connect(rest_, &Rest::succeeded, this, [&] { emit populated(); });
}

void CustomerModel::fetchAllCustomers()
{
    using UserTypes = UserEntity::UserTypes;
    const auto userType(CurrentUser::instance()->userType());
    Q_ASSERT(userType == UserTypes::ADMIN);

    rest_->get("/");

    connect(rest_, &Rest::succeeded, this, [this](const QVariantMap &resp) {
        // TODO: Add to necessary entites a static fromJson method that creates
        // a new entity.
        for (const auto &customer : resp["customers"].toList())
        {
            const auto customerMap{customer.toMap()};
            const auto customerEntity{new CustomerEntity};
            customerEntity->setCustomerId(customerMap["id"].toInt());
            customerEntity->setForename(customerMap["person_info"].toMap()["name"].toString());

            for (const auto &order : customerMap["orders"].toList())
                customerEntity->bills()->append(BillEntity::fromMap(order.toMap()));

            append(customerEntity);
        }

        emit allCustomersFetched(resp);
    });
}

void CustomerModel::fetchCustomerById(int customerId)
{
    Q_ASSERT(customerId);

    qDebug() << "BillBusiness::fetchCustomerById " << customerId;

    rest_->get(QString("/%1").arg(customerId));

    connect(rest_, &Rest::succeeded, this, [this, customerId](const QVariantMap &resp) {
        const auto customer{resp["customer"].toMap()};

        const auto customerEntity{new CustomerEntity};
        customerEntity->setCustomerId(customerId);
        customerEntity->setForename(customer["name"].toString());

        for (const auto &order : customer["orders"].toList())
            customerEntity->bills()->append(BillEntity::fromMap(order.toMap()));

        append(customerEntity);
    });
}

void CustomerModel::append(CustomerEntity * const customer)
{
    insert(count(), customer);
}

void CustomerModel::insert(int index, CustomerEntity * const customer)
{
    if (index < 0 || index > count())
        return;

    emit beginInsertRows(QModelIndex(), index, index);
    customer->setParent(this);
    data_.insert(index, customer);
    emit endInsertRows();
}

void CustomerModel::clear()
{
    emit beginResetModel();
    data_.clear();
    emit endResetModel();
}

QHash<int, QByteArray> CustomerModel::roleNames() const
{
    return roleNames_;
}
