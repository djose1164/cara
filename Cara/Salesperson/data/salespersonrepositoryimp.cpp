#include "salespersonrepositoryimp.h"
#include <QJsonDocument>
#include <QNetworkReply>
#include "networkrequestsingleton.h"
#include "salesperson.h"

using namespace Qt::StringLiterals;

namespace
{
constexpr auto resourceIdentification = "/%1"_L1;
constexpr auto queueEndpoint{"/queue"_L1};
constexpr auto orderEndpoint{"orders"_L1};
}

namespace Data {
SalespersonRepositoryImpl::SalespersonRepositoryImpl(QObject *parent)
    : QObject{parent}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{nullptr}
    , path_{"salespersons"_L1}
    , buffer_{}
    , api_{Authentication::NetworkRequestSingleton::instance().api()}
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

void SalespersonRepositoryImpl::fetchSalesperson(SalespersonId salespersonId)
{
    const auto resourceId{
        resourceIdentification.arg(QString::number(salespersonId))
    };

    reply_ = manager_->get(api_->createRequest(path_ + resourceId));

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

QNetworkReply *SalespersonRepositoryImpl::takeOrder(OrderId orderId, SalespersonId salespersonId)
{
    QJsonObject obj;
    obj["order_id"] = orderId;
    obj["salesperson_id"] = salespersonId;

    return manager_->post(
        api_->createRequest(orderEndpoint + queueEndpoint),
        QJsonDocument{obj}.toJson()
    );
}

QNetworkReply *SalespersonRepositoryImpl::abandonOrder(OrderId orderId)
{
    return manager_->deleteResource(api_->createRequest(
        orderEndpoint
        + resourceIdentification.arg(QString::number(orderId))
        + queueEndpoint
    ));
}

QNetworkReply *SalespersonRepositoryImpl::markOrderAsDone(OrderId orderId)
{
    QJsonObject obj;
    obj["is_done"] = true;

    return manager_->sendCustomRequest(
        api_->createRequest(orderEndpoint
            + resourceIdentification.arg(QString::number(orderId))
            + queueEndpoint),
        "PATCH"_ba,
        QJsonDocument{std::move(obj)}.toJson()
    );
}

QNetworkReply *SalespersonRepositoryImpl::fetchSalespeopleByAdminId(AdminId adminId)
{
    QUrlQuery query{ {"admin_id"_L1, QString::number(adminId)} };
    return manager_->get(api_->createRequest(path_, std::move(query)));
}

QNetworkReply *SalespersonRepositoryImpl::canOrderBePaid(SalespersonId salespersonId, OrderId orderId)
{
    return manager_->get(
        api_->createRequest(path_
            + resourceIdentification.arg(QString::number(salespersonId))
            + resourceIdentification.arg(QString::number(orderId))
        )
    );
}

void SalespersonRepositoryImpl::requestFinished()
{
    buffer_ = reply_->readAll();
    emit finished();

    if (reply_->error() != QNetworkReply::NoError)
    {
        emit errorOccurred();
        return;
    }

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


void Data::SalespersonRepositoryImpl::remove(int id) {}
