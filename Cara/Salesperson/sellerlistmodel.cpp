#include "sellerlistmodel.h"

using namespace Qt::StringLiterals;

namespace Seller
{
SellerListModel::SellerListModel(QObject *parent)
    : QAbstractListModel{parent}
    , data_{}
{}

int SellerListModel::rowCount(const QModelIndex &parent) const
{
    return count();
}

QVariant SellerListModel::data(const QModelIndex &index, int role) const
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
    case Roles::ForenameRole:
        return data_.at(row).forename();
    case Roles::SurnameRole:
        return data_.at(row).surname();
    case Roles::FullnameRole:
        return "%1 %2"_L1.arg(data_.at(row).forename(), data_.at(row).surname());
    case Roles::SellerIdRole:
        return data_.at(row).salespersonId();
    }
    return {};
}

bool SellerListModel::setData(const QModelIndex &index, const QVariant &value, int role)
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

QHash<int, QByteArray> SellerListModel::roleNames() const
{
    static const QHash<int, QByteArray> hash{
        {int(Roles::CreditAvailableRole), "creditAvailable"_ba},
        {int(Roles::CreditConsumedRole), "creditConsumed"_ba},
        {int(Roles::CreditLimitRole), "creditLimit"_ba},
        {int(Roles::SalespersonIdRole), "salespersonId"_ba},
        {int(Roles::SalespersonTypeRole), "salespersonType"_ba},
        {int(Roles::ForenameRole), "forename"_ba},
        {int(Roles::SurnameRole), "surname"_ba},
        {int(Roles::FullnameRole), "fullname"_ba},
        {int(Roles::SellerIdRole), "sellerId"_ba},
    };

    return hash;
}

int SellerListModel::count() const
{
    return data_.count();
}

void SellerListModel::insert(int index, const Entity::Salesperson &salesperson)
{
    if (index < 0 || index > count())
        return;

    beginInsertRows(QModelIndex(), index, index);
    data_.insert(index, salesperson);
    endInsertRows();

    emit countChanged();
}

void SellerListModel::append(const QVariantMap &salesperson)
{
    insert(count(), Entity::Salesperson::fromMap(salesperson));
}

void SellerListModel::setSellers(const QList<Entity::Salesperson> &sellerList)
{
    qDebug() << "SellerListModel::setSellers: sellerList.count(): " << sellerList.count();
    beginResetModel();
    data_ = sellerList;
    endResetModel();
    emit countChanged();
}

} // namespace Seller

