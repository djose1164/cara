#include "salespersoncontroller.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include "data/salespersonrepositoryimp.h"
#include "salesperson.h"

namespace Salesperson {
SalespersonController::SalespersonController(QObject *parent)
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

    connect(repository, &RepositoryImpl::errorOccurred,
            this, &SalespersonController::errorOccurred);
    connect(repository, &RepositoryImpl::fetched,
            this, &SalespersonController::fetched);
    connect(repository, &RepositoryImpl::patched,
            this, &SalespersonController::patched);
}

void SalespersonController::readSalesperson()
{
    salespersonRepository_->fetchSalesperson(adminId_);
}

void SalespersonController::readSalespeople()
{
    const auto reply{salespersonRepository_->fetchSalespeopleByAdminId(adminId_)};
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        setData(reply->readAll());

        if (reply->error() != QNetworkReply::NoError)
        {
            setErrorString(reply->errorString());
            emit errorOccurred();
            return;
        }

        emit fetched();

        reply->deleteLater();
    });
}

void SalespersonController::addAssociateSalesperson(const QVariantMap &data)
{
    const auto salesperson{Entity::Salesperson::fromMap(data)};
    if (salesperson.isPosteable())
        salespersonRepository_->addAssociateSalesperson(salesperson);
    else
        qDebug() << "Salesperson is not valid to be posteable";
    
    using RepositoryImpl = Data::SalespersonRepositoryImpl;
    const auto repository{static_cast<RepositoryImpl *>(salespersonRepository_)};
    connect(repository, &RepositoryImpl::added, this, &SalespersonController::added);
}

void SalespersonController::fetchAssociateSalespersonById(int id)
{
    salespersonRepository_->getSalespersonById(id);
}

void SalespersonController::fetchCustomers()
{
    if (salespersonId_ < 1)
    {
        emit errorOccurred();
        return;
    }
    salespersonRepository_->getCustomers(salespersonId_);
}

void SalespersonController::patch(const QVariantMap &data)
{
    Q_ASSERT(!data.isEmpty());
    qDebug() << "AssociateSalespersonService::patch: " << data;
    const auto payload{QJsonObject::fromVariantMap(data)};

    const auto doc{QJsonDocument(payload)};

    salespersonRepository_->patch(salespersonId_, doc.toJson(QJsonDocument::Compact));
}

void SalespersonController::takeOrder(OrderId orderId)
{
    const auto reply{salespersonRepository_->takeOrder(orderId, salespersonId_)};
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        setData(reply->readAll());
        const auto respObj{QJsonDocument::fromJson(data_).object()};
        if (reply->error() != QNetworkReply::NoError
            or respObj["status"].toString() != QStringLiteral("success"))
        {
            setErrorString(reply->errorString());
            emit errorOccurred();
        }

        emit orderTook();

        reply->deleteLater();
    });
}

void SalespersonController::abandonOrder(OrderId orderId)
{
    const auto reply{salespersonRepository_->abandonOrder(orderId)};
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        setData(reply->readAll());
        if (reply->error() != QNetworkReply::NoError)
        {
            setErrorString(reply->errorString());
            emit errorOccurred();
        }

        emit orderAbandoned();

        reply->deleteLater();
    });
}

void SalespersonController::markOrderAsDone(OrderId orderId)
{
    const auto reply{salespersonRepository_->markOrderAsDone(orderId)};
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        setData(reply->readAll());
        if (reply->error() != QNetworkReply::NoError)
        {
            setErrorString(reply->errorString());
            emit errorOccurred();
        }

        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) == 204)
            emit orderMarkedAsDone();

        reply->deleteLater();
    });
}

void SalespersonController::canPayOrder(OrderId orderId)
{
    const auto reply{salespersonRepository_->canOrderBePaid(salespersonId_, orderId)};
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        setData(reply->readAll());
        if (reply->error() != QNetworkReply::NoError)
        {
            setErrorString(reply->errorString());
            emit errorOccurred();
        }

        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) == 200)
            emit orderCanBePaid();

        reply->deleteLater();
    });
}

QString SalespersonController::errorString() const
{
    return errorString_;
}

void SalespersonController::setErrorString(const QString &newErrorString)
{
    if (errorString_ == newErrorString)
        return;
    errorString_ = newErrorString;
    emit errorStringChanged();
}

int SalespersonController::adminId() const
{
    return adminId_;
}

void SalespersonController::setAdminId(int newAdminId)
{
    if (adminId_ == newAdminId)
        return;
    adminId_ = newAdminId;
    emit adminIdChanged();
}

Entity::Salesperson SalespersonController::fromJsonObject(const QJsonObject &data) const
{
    return Entity::Salesperson::fromJson(
        QJsonDocument{data}.toJson()
    );
}

int SalespersonController::salespersonId() const
{
    return salespersonId_;
}

void SalespersonController::setSalespersonId(int newSalespersonId)
{
    if (salespersonId_ == newSalespersonId)
        return;
    salespersonId_ = newSalespersonId;
    emit salespersonIdChanged();
}

QByteArray SalespersonController::data() const
{
    return data_;
}

void SalespersonController::setData(const QByteArray &newData)
{
    if (data_ == newData)
        return;
    data_ = newData;
    emit dataChanged();
}
}
