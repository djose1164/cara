#include "ordermodel.h"

using EntityLayer::DetailsEntity;
using namespace Qt::StringLiterals;

namespace Model
{
OrderModel::OrderModel(QObject *parent)
    : QAbstractListModel{parent}
    , data_{}
{
    roleNames_[int(RoleNames::ProductRole)] = "product"_ba;
    roleNames_[int(RoleNames::TotalRole)] = "total"_ba;
    roleNames_[int(RoleNames::QuantityRole)] = "quantity"_ba;
    roleNames_[int(RoleNames::PriceRole)] = "price"_ba;
}

int OrderModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return data_.count();
}

QVariant OrderModel::data(const QModelIndex &index, int role) const
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

    case RoleNames::PriceRole:
        return data_[row]->price();

    default:
        qWarning() << "Trying to use a non-existent role name";
        break;
    }

    return {};
}

QHash<int, QByteArray> OrderModel::roleNames() const
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

QByteArray OrderModel::toJson() const
{
    QJsonArray arr{};
    for (const auto &detail : data_)
    {
        const auto detailJson{detail->toJson()};
        const auto detailObj{QJsonDocument::fromJson(detailJson).object()};
        arr.append(detailObj);
    }

    QJsonDocument doc{arr};
    qDebug() << "OrderModel::toJson " << doc;
    return doc.toJson(QJsonDocument::Compact);
}

void OrderModel::insert(int index, DetailsEntity *const entity)
{
    if (index < 0 || index > count())
        return;

    beginInsertRows(QModelIndex{}, index, index);
    data_.insert(index, entity);
    endInsertRows();

    emit countChanged(count());
}

void OrderModel::append(DetailsEntity *entity)
{
    insert(count(), entity);
}

void OrderModel::remove(int index)
{
    if (index < 0 || index > count())
        return;

    beginRemoveRows(QModelIndex{}, index, index);
    data_.remove(index);
    endRemoveRows();

    emit countChanged(count());
}

void OrderModel::clear()
{
    beginResetModel();
    data_.clear();
    endResetModel();
}

bool OrderModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    const int row{index.row()};
    switch(static_cast<RoleNames>(role))
    {
    case RoleNames::QuantityRole:
        data_[row]->setQuantity(value.toInt());
        emit dataChanged(index, index, {role, int(RoleNames::TotalRole)});
        return true;

    case RoleNames::ProductRole:
        break;

    case RoleNames::TotalRole:
        qWarning() << "Trying to set 'total' which is a readonly property";
        break;
    case RoleNames::PriceRole:
        break;
    }

    return false;
}
}
