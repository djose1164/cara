#include "networkrequestsingleton.h"
#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkRequestFactory>

using namespace Qt::StringLiterals;

static constexpr auto TRANSFER_TIMEOUT{std::chrono::minutes(1)};

namespace Utils
{
NetworkRequestSingleton &NetworkRequestSingleton::instance()
{
    static NetworkRequestSingleton nr{};
    return nr;
}

NetworkRequestSingleton::NetworkRequestSingleton()
    : api_{std::make_shared<QNetworkRequestFactory>(QSettings().value("base").toString())}
{
    const QString token{QSettings().value("Credentials/accessToken"_L1).toString()};
    api_->setBearerToken(token.toUtf8());

    QHttpHeaders headers{};
    headers.append(QHttpHeaders::WellKnownHeader::ContentType, "application/json"_L1);

    api_->setCommonHeaders(headers);

    api_->setTransferTimeout(TRANSFER_TIMEOUT);
}

QString NetworkRequestSingleton::accessToken() const
{
    return api_->bearerToken();
}

void NetworkRequestSingleton::setAccessToken(const QString &newAccessToken)
{
    api_->setBearerToken(newAccessToken.toUtf8());
}

std::shared_ptr<QNetworkRequestFactory> NetworkRequestSingleton::api() const
{
    return api_;
}
}
