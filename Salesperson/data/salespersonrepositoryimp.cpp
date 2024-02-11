#include "salespersonrepositoryimp.h"
#include "networkrequestsingleton.h"
#include "salesperson.h"
#include <QNetworkReply>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto resourceIdentification = "/%1"_L1;
}

namespace Data {
SalespersonRepositoryImpl::SalespersonRepositoryImpl(QObject *parent)
    : QObject{parent}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{nullptr}
    , path_{"salespersons"_L1}
    , buffer_{}
    , api_{Utils::NetworkRequestSingleton::instance().api()}
{
}

void SalespersonRepositoryImpl::getSalespersonById(int id)
{
    const auto request{
        api_->createRequest(path_
                            + resourceIdentification.arg(QString::number(id)))
    };

    reply_ = manager_->get(request);

    connect(reply_, &QNetworkReply::finished,
            this, &SalespersonRepositoryImpl::requestFinished);
}

void SalespersonRepositoryImpl::addAssociateSalesperson(const Entity::Salesperson &salesperson)
{
    reply_ = manager_->post(api_->createRequest(path_ + "/"_L1), salesperson.toJson());

    connect(reply_, &QNetworkReply::finished,
            this, &SalespersonRepositoryImpl::requestFinished);
}

void SalespersonRepositoryImpl::getAssociateSalespersons()
{
    reply_ = manager_->get(api_->createRequest(path_ + "/"_L1));

    connect(reply_, &QNetworkReply::finished,
            this, &SalespersonRepositoryImpl::requestFinished);
}

void SalespersonRepositoryImpl::patch(int id, const QByteArray &payload)
{
    const auto request{
        api_->createRequest(path_
                            + resourceIdentification.arg(QString::number(id)))
    };

    reply_ = manager_->sendCustomRequest(request, "PATCH", payload);

    connect(reply_, &QNetworkReply::finished,
            this, &SalespersonRepositoryImpl::requestFinished);
}

void SalespersonRepositoryImpl::getCustomers(int salespersonId)
{
    const auto request{api_->createRequest(path_
        + resourceIdentification.arg(QString::number(salespersonId))
        + "/customers"_L1
    )};

    reply_ = manager_->get(request);

    connect(reply_, &QNetworkReply::finished,
            this, &SalespersonRepositoryImpl::requestFinished);
}

void SalespersonRepositoryImpl::requestFinished()
{
    buffer_ = reply_->readAll();
    emit finished();

    if (reply_->error() != QNetworkReply::NoError)
        emit errorOccurred();

    switch(reply_->operation())
    {
    case QNetworkAccessManager::GetOperation:
        emit fetched();
        break;
    case QNetworkAccessManager::PostOperation:
        emit added();
        break;
    case QNetworkAccessManager::CustomOperation:
    {
        const auto verb{
            reply_->attribute(QNetworkRequest::CustomVerbAttribute).toString()
        };
        if (verb == "PATCH")
            emit patched();
        break;
    }
    case QNetworkAccessManager::HeadOperation:
    case QNetworkAccessManager::PutOperation:
    case QNetworkAccessManager::DeleteOperation:
    case QNetworkAccessManager::UnknownOperation:
        break;
    }

    reply_->deleteLater();
}

QString SalespersonRepositoryImpl::path() const
{
    return path_;
}

void SalespersonRepositoryImpl::setPath(const QString &newPath)
{
    path_ = newPath;
}

QByteArray SalespersonRepositoryImpl::buffer() const
{
    return buffer_;
}
}
