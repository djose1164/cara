#include "customermodel.h"
#include <QJsonDocument>
#include <QJsonArray>

namespace Model {
CustomerModel::CustomerModel(QObject *parent)
    : QAbstractListModel{parent}
{}

int CustomerModel::count() const
{
    return data_.count();
}

void CustomerModel::setRepository(Business::CustomerRepository *const newRepository)
{
    if (repository_ == newRepository)
        return;
    repository_ = newRepository;
    emit repositoryChanged();
}

Business::CustomerRepository *CustomerModel::repository() const
{
    return repository_;
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

    switch (static_cast<RoleNames>(role))
    {
    case RoleNames::CustomerIdRole:
        return object.value("customer_id").toInt();
    case RoleNames::ContactRole:
        return object.value("contact").toObject();
    case RoleNames::FullnameRole:
        return object.value("name").toString();
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
        {int(RoleNames::FullnameRole), "fullname"}
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
    data_.append(data);
}

void CustomerModel::update(SalespersonId salespersonId)
{
    if (!repository_)
    {
        qWarning("CustomerModel::update: a repository needs to be set.");
        return;
    }

    repository_->readBySalespersonId(salespersonId);

    connect(repository_, &Business::CustomerRepository::fetched, this, [&](const QByteArray &data) {
        const auto obj{QJsonDocument::fromJson(data).object()};
        const auto customersValue{obj.value("customers")};
        if (!customersValue.isArray())
        {
            qWarning() << "CustomerModel::update: customers is not an array.";
            return;
        }

        const auto customersArray = customersValue.toArray();

        beginResetModel();
        data_.clear();
        for (const auto customerValue : std::as_const(customersArray))
            data_.append(customerValue.toObject());
        endResetModel();
        emit countChanged();
        emit updated();
    });
}
}
