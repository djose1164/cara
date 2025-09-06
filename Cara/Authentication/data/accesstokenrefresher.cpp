#include "accesstokenrefresher.h"
#include "networkrequestsingleton.h"
#include "qnamsingleton.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequestFactory>
#include <QRestReply>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto tokenField{"access_token"_L1};
}

namespace Authentication::Data
{
AccessTokenRefresher::AccessTokenRefresher(QObject *parent)
    : Authentication::AbstractAccessTokenRefresher{parent}
    , manager_{QNAMSingleton::instance().qram()}
    , reply_{nullptr}
    , refreshToken_{}
    , api_{Network::NetworkRequestSingleton::instance().api()}
{
}

std::optional<QNetworkReply *> AccessTokenRefresher::refresh()
{
    if (refreshToken_.isEmpty())
    {
        constexpr auto error{"Refresh token is empty."_L1};
        qWarning() << "Refresher::refresh: "_L1 + error;
        return std::nullopt;
    }

    QNetworkRequest request(api_->createRequest("users/refresh/"_L1));
    request.setRawHeader("Authorization"_ba, "Bearer "_ba + refreshToken_.toUtf8());

    return manager_->get(request, this, [this](QRestReply &reply) {
        if (not reply.isSuccess())
        {
            const auto errmsg{"Failed to refresh token! Error: %1"_L1.arg(reply.errorString())};
            qWarning() << "Refresher::refresh: " << errmsg;
            emit errorOccurred(errmsg.toUtf8());
            return;
        }

        if (const auto json{reply.readJson()};
            json and json->isObject() and json->object().contains("access_token"_L1))
        {
            const QString token{json->object()[tokenField].toString()};
            setAccessToken(token);
            emit refreshed(token);
        }
    });
}

QString AccessTokenRefresher::refreshToken() const
{
    return refreshToken_;
}

void AccessTokenRefresher::setRefreshToken(const QString &newRefreshToken)
{
    refreshToken_ = newRefreshToken;
}

void AccessTokenRefresher::setAccessToken(const QString &newAccessToken)
{
    accessToken_ = newAccessToken;
}

QBindable<QString> AccessTokenRefresher::bindableAccessToken()
{
    return &accessToken_;
}

}
