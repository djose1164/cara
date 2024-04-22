#include "restorderrepository.h"
#include "networkrequestsingleton.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

using namespace Qt::StringLiterals;

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

void RestOrderRepository::add(const QVariantMap &payload)
{
    const auto api{Authentication::NetworkRequestSingleton::instance().api()};
    const auto req{api->createRequest("orders/"_L1)};

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
