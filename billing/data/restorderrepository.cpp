#include "restorderrepository.h"
#include "localuserstore.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

namespace Data
{
RestOrderRepository::RestOrderRepository(QObject *parent)
    : OrderRepository{parent}
    , manager_{new QNetworkAccessManager(this)}
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
    const auto jsonPayload{QJsonDocument::fromVariant(payload)};
    reply_ = manager_->post(req, jsonPayload.toJson(QJsonDocument::Compact));

    connect(reply_, &QNetworkReply::finished, this, [&] {
        if (reply_->error() != QNetworkReply::NoError)
        {
            setErrorString(reply_->errorString());
            emit errorOccurred();
            return;
        }
        const auto data{reply_->readAll()};
        const auto doc{QJsonDocument::fromJson(data)};
        emit resourceAdded(doc.object());
    });
}
}
