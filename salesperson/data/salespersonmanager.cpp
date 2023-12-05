#include "salespersonmanager.h"
#include "tokenmanager.h"
#include <QNetworkReply>

namespace Data
{
SalespersonManager::SalespersonManager(QObject *parent)
    : Business::Manageable{parent}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{}
{}

void SalespersonManager::read(int id)
{
    const auto req{prepareRequest(QString("/%1").arg(id))};

    reply_ = manager_->get(req);

    connect(reply_, &QNetworkReply::finished, this, [&] {
        setBuffer(reply_->readAll());
        if (reply_->error() == QNetworkReply::NoError)
            emit fetched();
        else
        {
            setErrorString(reply_->errorString());
            emit errorOccurred();
        }
        reply_->deleteLater();
    });
}

void SalespersonManager::readAll(int adminId)
{
    const auto req{prepareRequest(QString("/?admin_id=%1").arg(adminId))};

    reply_ = manager_->get(req);

    connect(reply_, &QNetworkReply::finished, this, [&] {
        setBuffer(reply_->readAll());
        if (reply_->error() == QNetworkReply::NoError)
            emit fetched();
        else
        {
            setErrorString(reply_->errorString());
            emit errorOccurred();
        }
        reply_->deleteLater();
    });
}

void SalespersonManager::add(const QByteArray &data)
{
    Q_ASSERT(data.contains("admin_id"));

    const auto req{prepareRequest("/")};

    reply_ = manager_->post(req, data);

    connect(reply_, &QNetworkReply::finished, this, [&] {
        setBuffer(reply_->readAll());
        if (reply_->error() == QNetworkReply::NoError)
            emit added();
        else
        {
            setErrorString(reply_->errorString());
            emit errorOccurred();
        }

        reply_->deleteLater();
    });
}

void SalespersonManager::edit(int id, const QByteArray &data)
{
}

void SalespersonManager::remove(int id)
{

}

QNetworkRequest SalespersonManager::prepareRequest(const QString &endpoint)
{
    QNetworkRequest req{QUrl(base() + endpoint)};
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    const QString token{DataLayer::TokenManager::instance()->accessToken()};
    req.setRawHeader("Authorization", token.toUtf8());

    return req;
}
}
