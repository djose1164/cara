#include "associatesalespersonservice.h"
#include "salesperson.h"
#include "data/salespersonrepositoryimp.h"
#include <QJsonObject>
#include <QJsonDocument>

namespace Salesperson {
AssociateSalespersonService::AssociateSalespersonService(QObject *parent)
    : QObject{parent}
    , salespersonRepository_{new Data::SalespersonRepositoryImpl(this)}
    , salespersonId_{}
    , adminId_{}
{
    using RepositoryImpl = Data::SalespersonRepositoryImpl;
    const auto repository{static_cast<RepositoryImpl *>(salespersonRepository_)};
    connect(repository, &RepositoryImpl::finished, this, [=, this] {
        setData(repository->buffer());
    });

    connect(repository, &RepositoryImpl::errorOccurred, this, &AssociateSalespersonService::errorOccurred);
    connect(repository, &RepositoryImpl::fetched, this, &AssociateSalespersonService::fetched);
    connect(repository, &RepositoryImpl::patched, this, &AssociateSalespersonService::patched);
}

void AssociateSalespersonService::fetchAllAssociateSalesperson()
{
    salespersonRepository_->getAssociateSalespersons();
}

void AssociateSalespersonService::addAssociateSalesperson(const QVariantMap &data)
{
    const auto salesperson{Entity::Salesperson::fromMap(data)};
    if (salesperson.isPosteable())
        salespersonRepository_->addAssociateSalesperson(salesperson);
    else
        qDebug() << "Salesperson is not valid to be posteable";
    
    using RepositoryImpl = Data::SalespersonRepositoryImpl;
    const auto repository{static_cast<RepositoryImpl *>(salespersonRepository_)};
    connect(repository, &RepositoryImpl::added, this, &AssociateSalespersonService::added);
}

void AssociateSalespersonService::fetchAssociateSalespersonById(int id)
{
    salespersonRepository_->getSalespersonById(id);
}

void AssociateSalespersonService::fetchCustomers()
{
    if (salespersonId_ < 1)
    {
        emit errorOccurred();
        return;
    }
    salespersonRepository_->getCustomers(salespersonId_);
}

void AssociateSalespersonService::patch(const QVariantMap &data)
{
    Q_ASSERT(!data.isEmpty());
    qDebug() << "AssociateSalespersonService::patch: " << data;
    const auto payload{QJsonObject::fromVariantMap(data)};

    const auto doc{QJsonDocument(payload)};

    salespersonRepository_->patch(salespersonId_, doc.toJson(QJsonDocument::Compact));
}

int AssociateSalespersonService::adminId() const
{
    return adminId_;
}

void AssociateSalespersonService::setAdminId(int newAdminId)
{
    if (adminId_ == newAdminId)
        return;
    adminId_ = newAdminId;
    emit adminIdChanged();
}

int AssociateSalespersonService::salespersonId() const
{
    return salespersonId_;
}

void AssociateSalespersonService::setSalespersonId(int newSalespersonId)
{
    if (salespersonId_ == newSalespersonId)
        return;
    salespersonId_ = newSalespersonId;
    emit salespersonIdChanged();
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
