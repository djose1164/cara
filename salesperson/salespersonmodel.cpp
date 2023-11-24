#include "salespersonmodel.h"

namespace Model
{
SalespersonModel::SalespersonModel(QObject *parent)
    : QAbstractListModel{parent}
    , data_{}
    , count_{}
{}

int SalespersonModel::rowCount(const QModelIndex &parent) const
{
    return count();
}

QVariant SalespersonModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    const int row{index.row()};
    switch (static_cast<Roles>(role))
    {
    case Roles::CreditAvailableRole:
        return data_.at(row).creditAvailable();
    case Roles::CreditConsumedRole:
        return data_.at(row).creditConsumed();
    case Roles::CreditLimitRole:
        return data_.at(row).creditLimit();
    case Roles::SalespersonIdRole:
        return data_.at(row).salespersonId();
    case Roles::SalespersonTypeRole:
        return int(data_.at(row).salespersonType());
    default:
        qWarning() << "The role " << role << " is not valid";
        break;
    }
    return {};
}

bool SalespersonModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return {};
    auto &salesperson{data_[index.row()]};
    switch (static_cast<Roles>(role))
    {
    case Roles::CreditAvailableRole:
        if (salesperson.creditAvailable() == value.toInt())
            return false;
        salesperson.setCreditAvailable(value.toInt());
        emit dataChanged(index, index, {role});
        return true;
    case Roles::CreditLimitRole:
        if (salesperson.creditLimit() == value.toInt())
            return false;
        salesperson.setCreditLimit(value.toInt());
        emit dataChanged(index, index, {role});
        return true;
    case Roles::CreditConsumedRole:
        if (salesperson.creditConsumed() == value.toInt())
            return false;
        salesperson.setCreditConsumed(value.toInt());
        emit dataChanged(index, index, {role});
        return true;
    default:
        break;
    }
    return {};
}

QHash<int, QByteArray> SalespersonModel::roleNames() const
{
    static const QHash<int, QByteArray> hash{
        {int(Roles::CreditAvailableRole), "creditAvailable"},
        {int(Roles::CreditConsumedRole), "creditConsumed"},
        {int(Roles::CreditLimitRole), "creditLimit"},
        {int(Roles::SalespersonIdRole), "salespersonId"},
        {int(Roles::SalespersonTypeRole), "salespersonType"}
    };

    return hash;
}

int SalespersonModel::count() const
{
    return data_.count();
}

void SalespersonModel::insert(int index, const Entity::Salesperson &salesperson)
{
    if (index < 0 || index > count())
        return;

    beginInsertRows(QModelIndex(), index, index);
    data_.insert(index, salesperson);
    endInsertRows();

    emit countChanged();
}

void SalespersonModel::append(const Entity::Salesperson &salesperson)
{
    insert(count(), salesperson);
}
}
