#include "restorderrepository.h"
#include "localuserstore.h"
#include <QJsonObject>
#include <QSettings>

namespace Data
{
RestOrderRepository::RestOrderRepository(QObject *parent)
    : OrderRepository{parent}
    , manager_{new QRestAccessManager(this)}
    , reply_{}
    , base_{QSettings().value("base").toString() + "/orders"}
    , errorString_{}
{}

QString RestOrderRepository::errorString() const
{
    return errorString_;
}

void RestOrderRepository::setErrorString(const QString &newErrorString)
{
    errorString_ = newErrorString;
}

QNetworkRequest RestOrderRepository::prepareRequest(const QString &endpoint)
{
    const QUrl url{base_ + endpoint};
    QNetworkRequest req{url};
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    const QString token{Business::LocalUserStore().user().accessToken()};
    req.setRawHeader("Authorization", token.toUtf8());
    return req;
}

void RestOrderRepository::add(const QVariantMap &payload)
{
    const auto req{prepareRequest()};
    manager_->post(req, payload, this, [&](QRestReply *reply) {
        if (!reply->isSuccess())
        {
            setErrorString(reply->errorString());
            emit errorOccurred();
            return;
        }
        if (const auto json = reply->json())
            emit resourceAdded(*json);
    });
}
}
