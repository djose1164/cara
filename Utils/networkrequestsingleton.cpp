#include "networkrequestsingleton.h"
#include <QSettings>

using namespace Qt::StringLiterals;

namespace Utils
{
NetworkRequestSingleton &NetworkRequestSingleton::instance()
{
    static NetworkRequestSingleton nr{};
    return nr;
}

NetworkRequestSingleton::NetworkRequestSingleton()
    : api_{std::make_shared<QNetworkRequestFactory>(QSettings().value("base").toString())}
    , manager_{std::make_shared<QNetworkAccessManager>()}
{
    const QString token{QSettings().value("Credentials/accessToken"_L1).toString()};
    api_->setBearerToken(token.toUtf8());

    QHttpHeaders headers{};
    headers.append(QHttpHeaders::WellKnownHeader::ContentType, "application/json"_L1);

    api_->setCommonHeaders(headers);
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

QNetworkAccessManager *NetworkRequestSingleton::manager() const
{
    return manager_.get();
}
}
