#include "billmodel.h"
#include "ordermodel.h"


namespace Model
{
BillModel::BillModel(QObject *parent)
    : QAbstractListModel(parent)
    , roleNames_{}
{
    roleNames_[int(RoleNames::UserRole)] = "user";
    roleNames_[int(RoleNames::DetailsRole)] = "details";
    roleNames_[int(RoleNames::OrderIdRole)] = "orderId";
    roleNames_[int(RoleNames::PaymentRole)] = "payment";
    roleNames_[int(RoleNames::DateRole)] = "date";
    roleNames_[int(RoleNames::StatusRole)] = "status";
    roleNames_[int(RoleNames::CustomerRole)] = "customer";
}

int BillModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return count();
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
    case RoleNames::StatusRole:
        return QVariant::fromValue(item->orderStatus());
    case RoleNames::CustomerRole:
        return QVariant::fromValue(item->customer());
    default:
        return {};
    }
}

int BillModel::count() const
{
    return data_.count();
}

EntityLayer::BillEntity *BillModel::get(int index) const
{
    if (index < 0 || index >= count())
        return nullptr;

    return data_.at(index);
}

bool BillModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    const int row{index.row()};
    switch (static_cast<RoleNames>(role)) {
        using enum RoleNames;
    case DateRole:
        data_[row]->setDate(value.toDateTime());
        emit dataChanged(index, index, {role});
        return true;
    case DetailsRole:
        data_[row]->setDetailsModel(value.value<OrderModel *>());
        emit dataChanged(index, index, {role});
        return true;
    case PaymentRole:
        data_[row]->setPayment(value.value<Entity::Payment>());
        emit dataChanged(index, index, {role});
        return true;
    default:
        qWarning() << "Trying to use a non-existent role name";
        return false;
    }
}

QHash<int, QByteArray> BillModel::roleNames() const
{
    return roleNames_;
}

void BillModel::insert(int index, BillEntity *const bill)
{
    if (index < 0 || index > count())
        return;

    beginInsertRows(QModelIndex(), index, index);
    bill->setParent(this);
    data_.insert(index, bill);
    endInsertRows();

    emit countChanged();
}

void BillModel::append(BillEntity * const bill)
{
    insert(count(), bill);
}

void BillModel::remove(int index)
{
    if (index < 0 || index >= count())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    data_.removeAt(index);
    endRemoveRows();
    emit countChanged();
}

void BillModel::clear()
{
    if (data_.isEmpty())
        return;

    beginResetModel();
    data_.clear();
    endResetModel();

    emit countChanged();
}
}
