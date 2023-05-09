#include "ordermodel.h"

using ModelLayer::OrderModel, EntityLayer::DetailsEntity;

OrderModel::OrderModel(QObject *parent)
    : QAbstractListModel{parent}
    , data_{}
{
    roleNames_[int(RoleNames::ProductRole)] = "product";
    roleNames_[int(RoleNames::TotalRole)] = "total";
    roleNames_[int(RoleNames::QuantityRole)] = "quantity";
}


int ModelLayer::OrderModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return data_.count();
}

QVariant ModelLayer::OrderModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    const auto row{index.row()};

    switch (static_cast<RoleNames>(role))
    {
    case RoleNames::ProductRole:
        return QVariant::fromValue(data_[row]->product());

    case RoleNames::QuantityRole:
        return data_[row]->quantity();

    case RoleNames::TotalRole:
        return data_[row]->total();

    default:
        qWarning() << "Trying to use a non-existent role name";
        break;
    }

    return {};
}


QHash<int, QByteArray> ModelLayer::OrderModel::roleNames() const
{
    return roleNames_;
}

int OrderModel::count() const
{
    return data_.count();
}

DetailsEntity *OrderModel::get(int index)
{
    if (index < 0 || index > count())
        return nullptr;
    return data_[index];
}

void OrderModel::insert(int index, DetailsEntity *entity)
{
    if (index < 0 || index > count())
        return;

    emit beginInsertRows(QModelIndex{}, index, index);
    entity->setParent(this);
    data_.insert(index, entity);
    emit endInsertRows();

    emit countChanged(count());
}

void OrderModel::append(EntityLayer::DetailsEntity * const entity)
{
    insert(count(), entity);
}

void OrderModel::remove(int index)
{
    if (index < 0 || index > count())
        return;

    emit beginRemoveRows(QModelIndex{}, index, index);
    data_.remove(index);
    emit endRemoveRows();

    emit countChanged(count());
}

void OrderModel::clear()
{
    data_.clear();
}
