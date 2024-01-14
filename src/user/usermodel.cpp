#include "usermodel.h"
#include "data/userrepositoryimpl.h"

namespace Model {
UserModel::UserModel(QObject *parent)
    : QAbstractListModel{parent}
    , data_{}
    , repository_{new Data::UserRepositoryImpl(this)}
{

}

int UserModel::count() const
{
    return data_.count();
}

int UserModel::rowCount(const QModelIndex &parent) const
{
    return count();
}

QVariant UserModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    const int row{index.row()};
    switch (static_cast<Roles>(role)) {
    case Roles::UsernameRole:
        return data_.at(row).username();
    case Roles::isAdminRole:
        return data_.at(row).isAdmin();
    case Roles::isSalespersonRole:
        return data_.at(row).isSalesperson();
    case Roles::isCustomerRole:
        return data_.at(row).isCustomer();
    case Roles::UserTypeRole:
        return int(data_.at(row).userType());
    case Roles::ForenameRole:
        return data_.at(row).forename();
    case Roles::SurnameRole:
        return data_.at(row).surname();
    case Roles::SalespersonRole:
        return QVariant::fromValue(data_.at(row).salesperson());
    default:
        break;
    }

    return {};
}

QHash<int, QByteArray> UserModel::roleNames() const
{
    static QHash<int, QByteArray> hash = {
        {int(Roles::UsernameRole), "username"},
        {int(Roles::PasswordRole), "password"},
        {int(Roles::ForenameRole), "forename"},
        {int(Roles::SurnameRole), "surname"},
        {int(Roles::UserIdRole), "userId"},
        {int(Roles::UserTypeRole), "userType"},
        {int(Roles::isAdminRole), "isAdmin"},
        {int(Roles::isCustomerRole), "isCustomer"},
        {int(Roles::isSalespersonRole), "isSalesperson"},
        {int(Roles::isGuestRole), "isGuest"},
        {int(Roles::SalespersonRole), "salesperson"}
    };
    return hash;
}

bool UserModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    const int row{index.row()};
    auto &userEntry{data_[row]};

    switch (static_cast<Roles>(role)) {
    case Roles::UsernameRole:
        if (userEntry.username() == value.toString())
            return false;
        userEntry.setUsername(value.toString());
        emit dataChanged(index, index, {role});
        return true;
    case Roles::PasswordRole:
        if (userEntry.password() == value.toString())
            return false;
        userEntry.setPassword(value.toString());
        emit dataChanged(index, index, {role});
        return true;
    case Roles::CustomerIdRole:
        if (userEntry.customerId() == value.toInt())
            return false;
        userEntry.setCustomerId(value.toInt());
        emit dataChanged(index, index, {role});
        return true;
    case Roles::SalespersonRole:
    {
        const auto &sp{value.value<Entity::Salesperson>()};
        qDebug() << "UserModel::setData: Setting new value for salesperson:"
                 << "ca: " << sp.creditAvailable();
        userEntry.setSalesperson(sp);
        emit dataChanged(index, index, {role});
        return true;
    }
    default:
        break;
    }

    return false;
}

void UserModel::insert(int index, const Entity::UserEntity &user)
{
    if(index < 0 || index > count())
        return;

    beginInsertRows(QModelIndex(), index, index);
    data_.insert(index, user);
    endInsertRows();

    emit countChanged();
}

void UserModel::append(const Entity::UserEntity &user)
{
    insert(count(), user);
}

void UserModel::clear()
{
    if (data_.isEmpty())
        return;

    beginResetModel();
    data_.clear();
    endResetModel();

    emit countChanged();
}
}
