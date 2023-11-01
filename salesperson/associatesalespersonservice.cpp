#include "associatesalespersonservice.h"
#include "entity/salesperson.h"
#include "data/associatesalespersonrepositoryimp.h"

namespace Salesperson {
AssociateSalespersonService::AssociateSalespersonService(QObject *parent)
    : QObject{parent}
    , salespersonRepository_{new Data::AssociateSalespersonRepositoryImpl(this)}
{
    using RepositoryImpl = Data::AssociateSalespersonRepositoryImpl;
    const auto repository{static_cast<RepositoryImpl *>(salespersonRepository_)};
    connect(repository, &RepositoryImpl::finished, this, [&](const QByteArray &newData) {
        setData(newData);
    });

    connect(repository, &RepositoryImpl::errorOccurred, this, &AssociateSalespersonService::errorOccurred);
    connect(repository, &RepositoryImpl::fetched, this, &AssociateSalespersonService::fetched);
    connect(repository, &RepositoryImpl::added, this, &AssociateSalespersonService::added);
}

void AssociateSalespersonService::fetchAllAssociateSalesperson()
{
    salespersonRepository_->getAssociateSalespersons();
}

void AssociateSalespersonService::addAssociateSalesperson(const QVariantMap &data)
{
    const auto salesperon{Entity::Salesperson::fromMap(data)};
    if (salesperon.isPosteable())
        salespersonRepository_->addAssociateSalesperson(salesperon);
    else
        qDebug() << "Salesperson is not valid to be posteable";
}

void AssociateSalespersonService::fetchAssociateSalespersonById(int id)
{

}

QByteArray AssociateSalespersonService::data() const
{
    return data_;
}

void AssociateSalespersonService::setData(const QByteArray &newData)
{
    if (data_ == newData)
        return;
    data_ = newData;
    emit dataChanged();
}
}
