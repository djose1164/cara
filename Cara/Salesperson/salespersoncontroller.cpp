#include "salespersoncontroller.h"
#include "data/salespersonrepositoryimp.h"
#include "salesperson.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

using namespace Qt::StringLiterals;

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
            this, &SalespersonController::sellersFetched);
    connect(repository, &RepositoryImpl::patched,
            this, &SalespersonController::patched);
}

void SalespersonController::readSalesperson()
{
    if (salespersonId_ < 1)
    {
        setErrorString("Invalid salespersonId."_L1);
        emit errorOccurred();
        return;
    }

    salespersonRepository_->fetchSalesperson(salespersonId_);
}

void SalespersonController::readSalespeople()
{
    if (adminId_ < 1)
    {
        setErrorString("Invalid adminId."_L1);
        emit errorOccurred();
        qWarning() << "SalespersonController::readSalespeople: " << errorString_;
        return;
    }

    const auto reply{salespersonRepository_->fetchSalespeopleByAdminId(adminId_)};
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        setData(reply->readAll());

        if (reply->error() != QNetworkReply::NoError)
        {
            setErrorString(reply->errorString());
            emit errorOccurred();
            return;
        }

        const auto salespeopleObject = QJsonDocument::fromJson(data_).object();
        const auto salespeopleArr = salespeopleObject["salespeople"_L1].toArray();
        QList<Entity::Salesperson> sellerList;
        std::ranges::for_each(salespeopleArr, [&](const auto &jsvalue) {
            sellerList.append(Entity::Salesperson::fromJsonObject(jsvalue.toObject()));
        });

        emit fetched(sellerList);

        reply->deleteLater();
    });
}

void SalespersonController::addAssociateSalesperson(const QVariantMap &data)
{
    const auto salesperson{Entity::Salesperson::fromMap(data)};
    if (salesperson.isPosteable())
        salespersonRepository_->addAssociateSalesperson(salesperson);
    else
        qDebug() << "Salesperson is not valid to be posteable: " << data;
    
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
    if (orderId < 1)
    {
        setData(QByteArrayLiteral("Invalid orderId."));
        emit errorOccurred();
        return;
    }

    const auto reply{salespersonRepository_->takeOrder(orderId, salespersonId_)};
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        setData(reply->readAll());
        const auto respObj{QJsonDocument::fromJson(data_).object()};

        if (reply->error() != QNetworkReply::NoError
            or respObj[QStringLiteral("code")].toString() != QStringLiteral("success"))
        {
            setErrorString(
                respObj["error"_L1].toString()
            );
            emit errorOccurred();
            return;
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

void SalespersonController::sellersFetched()
{
    if (const auto json{QJsonDocument::fromJson(data_)};
        json.isObject() and json.object().contains("salespeople"_L1))
    {
        const QJsonArray salespeople = json.object()["salespeople"_L1].toArray();
        QList<Entity::Salesperson> sellerList;
        std::ranges::for_each(salespeople, [&](const auto &jsvalue) {
            auto sellerJson{QJsonDocument{jsvalue.toObject()}.toJson()};
            auto seller{Entity::Salesperson::fromJson(sellerJson)};
            sellerList.append(std::move(seller));
        });
        emit fetched(sellerList);
    }
    else
    {
        emit fetched();
    }
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
