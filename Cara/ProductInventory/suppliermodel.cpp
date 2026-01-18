#include "suppliermodel.h"

using namespace Qt::StringLiterals;

namespace ProductInventory
{

SupplierModel::SupplierModel(QObject *parent)
    : QAbstractListModel{parent}
    , data_{}
    , repository_{}
{
}

int SupplierModel::rowCount(const QModelIndex &parent) const
{
    return data_.count();
}

QVariant SupplierModel::data(const QModelIndex &index, int role) const
{
    if (not index.isValid())
        return {};

    const auto at{data_.at(index.row())};
    switch (static_cast<RoleNames>(role))
    {
        using enum RoleNames;
    case NameRole:
        return at.name();
    case UniqueCodeRole:
        return at.uniqueCode();
    case SupplierIdRole:
        return at.supplierId();
    }
    return {};
}

QHash<int, QByteArray> SupplierModel::roleNames() const
{
    using enum RoleNames;
    return {{int(NameRole), "name"_ba},
            {int(UniqueCodeRole), "uniqueCode"_ba},
            {int(SupplierIdRole), "supplierId"_ba}};
}

AbstractSupplierRepository *SupplierModel::repository() const
{
    return repository_;
}

void SupplierModel::setRepository(AbstractSupplierRepository *newRepository)
{
    if (repository_)
        repository_->disconnect(this);

    if (repository_ == newRepository)
        return;
    repository_ = newRepository;

    if (repository_)
    {
        connect(repository_,
                &AbstractSupplierRepository::suppliersFetched,
                this,
                &SupplierModel::reset);
    }
    emit repositoryChanged();
}

void SupplierModel::reset(const QList<Supplier> &suppliers)
{
    qDebug() << "SupplierModel::reset: Resetting suppliers in model...";
    beginResetModel();
    data_.clear();
    data_ = suppliers;
    endResetModel();
}

void ProductInventory::SupplierModel::fetchSuppliers()
{
    if (not repository_)
    {
        qDebug()
            << "ProductInventory::SupplierModel::fetchSuppliers: the repository hasn't been set!";
    }

    repository_->fetchSuppliers();
}

} // namespace ProductInventory
