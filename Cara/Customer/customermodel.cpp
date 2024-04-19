#include "customermodel.h"
#include <QJsonDocument>
#include <QJsonArray>

namespace Model {
CustomerModel::CustomerModel(QObject *parent)
    : QAbstractListModel{parent}
    , data_{}
{}

int CustomerModel::count() const
{
    return data_.count();
}

int CustomerModel::rowCount(const QModelIndex &parent) const
{
    return data_.count();
}

QVariant CustomerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    const int idx{index.row()};
    const auto object{data_.at(idx)};
    const auto contactObject{object["contact"].toObject()};

    switch (static_cast<RoleNames>(role))
    {
    case RoleNames::CustomerIdRole:
        return object.value("customer_id").toInt();
    case RoleNames::ContactRole:
        return object.value("contact").toObject();
    case RoleNames::FullnameRole:
        return object.value("name").toString();
    case RoleNames::ForenameRole:
        return contactObject["forename"].toString();
    case RoleNames::SurnameRole:
        return contactObject["surname"].toString();
    default:
        break;
    }

    return {};
}

QHash<int, QByteArray> Model::CustomerModel::roleNames() const
{
    static const QHash<int, QByteArray> hash{
        {int(RoleNames::CustomerIdRole), "customerId"},
        {int(RoleNames::AddressRole), "address"},
        {int(RoleNames::ContactRole), "contact"},
        {int(RoleNames::SalespersonRole), "salesperson"},
        {int(RoleNames::FullnameRole), "fullname"},
        {int(RoleNames::ForenameRole), "forename"},
        {int(RoleNames::SurnameRole), "surname"}
    };
    return hash;
}

void CustomerModel::insert(int index, const QJsonObject &data)
{
    if (index < 0 || index > count())
        return;

    beginInsertRows(QModelIndex(), index, index);
    data_.insert(index, data);
    endInsertRows();

    emit countChanged();
}

void CustomerModel::append(const QJsonObject &data)
{
    insert(count(), data);
}

void CustomerModel::clear()
{
    beginResetModel();
    data_.clear();
    endResetModel();
}

void CustomerModel::set(const QJsonArray &data)
{
    beginResetModel();
    data_.clear();
    for (const auto customerValue : std::as_const(data))
        data_.append(customerValue.toObject());
    endResetModel();
    emit countChanged();
    emit updated();
}
}
