#include "billmodel.h"

using ModelLayer::BillModel;

BillModel::BillModel(QObject *parent)
    : QAbstractListModel(parent)
{
    roleNames_[int(RoleNames::UserRole)] = "user";
    roleNames_[int(RoleNames::DetailsRole)] = "details";
    roleNames_[int(RoleNames::OrderIdRole)] = "orderId";
    roleNames_[int(RoleNames::PaymentRole)] = "payment";
    roleNames_[int(RoleNames::DateRole)] = "date";
}

int BillModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return data_.count();
}

QVariant BillModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const int row{index.row()};
    const auto item{data_[row]};

    switch (static_cast<RoleNames>(role)) {
    case RoleNames::DateRole:
        return item->date();
    case RoleNames::DetailsRole:
        return QVariant::fromValue(item->details());
    case RoleNames::OrderIdRole:
        return item->orderId();
    case RoleNames::PaymentRole:
        return QVariant::fromValue(item->payment());
    case RoleNames::UserRole:
        return QVariant::fromValue(item->user());
    default:
        break;
    }

    return {};
}

int BillModel::count() const
{
    return count_.value();
}

QBindable<int> BillModel::bindableCount()
{
    return &count_;
}

bool BillModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    const int row{index.row()};
    switch (static_cast<RoleNames>(role)) {
    case RoleNames::DateRole:
        data_[row]->setDate(value.toDate());
        emit dataChanged(index, index, {role});
        break;
    case RoleNames::DetailsRole:
        data_[row]->setDetailsModel(value.value<QList<DetailsEntity *>>());
        emit dataChanged(index, index, {role});
        break;
    default:
        qWarning() << "Trying to use a non-existent role name";
        break;
    }

    return false;
}

QHash<int, QByteArray> BillModel::roleNames() const
{
    return roleNames_;
}

void BillModel::insert(int index, BillEntity *const bill)
{
    if (!indexIsValid(index))
        return;
    data_.insert(index, bill);
}

void BillModel::append(BillEntity * const bill)
{
    insert(count(), bill);
}

void BillModel::remove(int index)
{
    data_.remove(index);
}

void BillModel::clear()
{
    data_.clear();
}

bool BillModel::indexIsValid(int index) const
{
    return !(index < 0 || index > data_.count());
}
