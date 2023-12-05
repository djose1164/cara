#include "salespersonrepositoryimp.h"
#include "tokenmanager.h"
#include <QNetworkReply>

namespace Data {
SalespersonRepositoryImpl::SalespersonRepositoryImpl(QObject *parent)
    : QObject{parent}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{nullptr}
{
    const QSettings settings;
    const QString urlBase{settings.value("base").toString()};

    Q_ASSERT(!urlBase.isEmpty());
    base_ = urlBase + "/salespersons/";
}

void SalespersonRepositoryImpl::getSalespersonById(int id)
{
    const QNetworkRequest request{prepareRequest(QString::number(id))};

    reply_ = manager_->get(request);

    connect(reply_, &QNetworkReply::finished, this, [&] {
        buffer_ = reply_->readAll();
        if (reply_->error() == QNetworkReply::NoError)
            emit fetched();
        else
            emit errorOccurred();

        reply_->deleteLater();
    });
}

void SalespersonRepositoryImpl::addAssociateSalesperson(const Entity::Salesperson &salesperson)
{
    const QNetworkRequest request{prepareRequest()};

    reply_ = manager_->post(request, salesperson.toJson());

    connect(reply_, &QNetworkReply::finished, this, [&] {
        buffer_ = reply_->readAll();
        emit finished();

        if (reply_->error() == QNetworkReply::NoError)
            emit added();
        else
            emit errorOccurred();     

        reply_->deleteLater();
    });
}

void SalespersonRepositoryImpl::getAssociateSalespersons()
{
    const QNetworkRequest request{prepareRequest()};

    reply_ = manager_->get(request);

    connect(reply_, &QNetworkReply::finished, this, [&] {
        buffer_ = reply_->readAll();
        emit finished();

        if (reply_->error() == QNetworkReply::NoError)
            emit fetched();
        else
            emit errorOccurred();

        reply_->deleteLater();
    });
}

void SalespersonRepositoryImpl::patch(int id, const QByteArray &payload)
{
    Q_ASSERT(!payload.isEmpty());
    const auto req{prepareRequest(QString::number(id))};

    reply_ = manager_->sendCustomRequest(req, "PATCH", payload);

    connect(reply_, &QNetworkReply::finished, this, [&] {
        buffer_ = reply_->readAll();
        emit finished();

        if (reply_->error() == QNetworkReply::NoError)
            emit patched();
        else
            emit errorOccurred();

        reply_->deleteLater();
    });
}

void SalespersonRepositoryImpl::getCustomers(int id)
{
    Q_ASSERT(id);

    const auto req{prepareRequest(QString("%1/customers").arg(id))};

    reply_ = manager_->get(req);

    connect(reply_, &QNetworkReply::finished, this, [&] {
        buffer_ = reply_->readAll();
        emit finished();

        if (reply_->error() == QNetworkReply::NoError)
            emit fetched();
        else
            emit errorOccurred();

        reply_->deleteLater();
    });
}

QNetworkRequest SalespersonRepositoryImpl::prepareRequest(const QString &endpoint) const
{
    QNetworkRequest request{endpoint.isEmpty() ? base_ : base_.toString() + endpoint};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    const auto token{DataLayer::TokenManager::instance()->accessToken().toUtf8()};
    request.setRawHeader("Authorization", token);
    return request;
}

QByteArray SalespersonRepositoryImpl::buffer() const
{
    return buffer_;
}
}
