#include "localuserauthentication.h"
#include "networkrequestsingleton.h"
#include "qnamsingleton.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QRestReply>
#include <QSettings>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto accessTokenField{"access_token"_L1};
constexpr auto userField{"user"_L1};
constexpr auto codeField{"code"_L1};
constexpr auto messageField{"message"_L1};
constexpr auto expiredCode{"refresh_token_expired"_L1};
constexpr auto invalidCode{"refresh_token_invalid"_L1};
constexpr auto requiredCode{"authorization_required"_L1};
}

namespace Authentication::Service {
LocalUserAuthentication::LocalUserAuthentication(QObject *parent)
    : Authenticator{parent}
    , refreshToken_{}
    , isAuthenticated_{}
    , manager_{QNAMSingleton::instance().qram()}
    , path_{"/users/refresh"_L1}
{
}

void LocalUserAuthentication::authenticate()
{
    qInfo() << "Authenticationg user through LocalUserAuthentication...";
    if (refreshToken_.trimmed().isEmpty())
    {
        constexpr auto errorString{"refreshToken is empty."_L1};
        qCritical() << "LocalUserAuthentication::authenticate: " << errorString;
        emit errorOccurred(Authenticator::Error::InvalidState, errorString);
        return;
    }

    QUrlQuery query{{"include_user"_L1, "true"_L1}};
    const auto api{Network::NetworkRequestSingleton::instance().api()};

    const QByteArray token{"Bearer "_ba + refreshToken_.toUtf8()};
    const QByteArray header{"Authorization"_ba};
    QNetworkRequest request{api->createRequest(path_, query)};
    request.setRawHeader(header, token.data());

    manager_->get(request, this, [&](QRestReply &reply) {
        qDebug() << "LocalUserAuthentication::authenticate: handling response";
        const auto json{reply.readJson()};

        if (not reply.isSuccess())
        {
            qDebug() << "LocalUserAuthentication::authenticate: " << *json;
            if (json and json->isObject() and json->object().contains(codeField)
                and json->object().contains(messageField))
            {
                const QString code{(*json)[codeField].toString()};
                const QString msg{(*json)[messageField].toString()};
                const QString errstr{"%1: %2"_L1.arg(code, msg)};
                qWarning() << "LocalUserAuthentication::authenticate: " << errstr;
                if (code == invalidCode or code == requiredCode)
                {
                    emit errorOccurred(Error::InvalidState, errstr);
                    return;
                }
                else if (code == expiredCode)
                {
                    emit tokenExpired();
                    return;
                }
            }
            else
            {
                // We assume there's an error the internet connection or with the server
                // itself.
                emit errorOccurred(Error::NetworkUnavailable, reply.errorString());
            }
        }

        isAuthenticated_ = true;

        if (json and json->isObject() and json->object().contains(accessTokenField)
            and json->object().contains(userField))
        {
            const auto obj{json->object()};
            if (!obj["access_token"_L1].isString())
            {
                qCritical() << "LocalUserAuthentication::authenticate: token isn't a string";
                emit errorOccurred(Error::Unknown, "token isn't a string"_L1);
                return ;
            }
            const auto tokenValue{obj["access_token"_L1].toString()};
            emit tokenGenerated(tokenValue);
            emit authenticationSucceeded(json->toJson());
        }

        qDebug() << "User authenticated successfully through LocalUserAuthentication.";
    });
}

QString LocalUserAuthentication::refreshToken() const
{
    return refreshToken_;
}

void LocalUserAuthentication::setRefreshToken(const QString &newRefreshToken)
{
    if (refreshToken_ == newRefreshToken)
        return;
    refreshToken_ = newRefreshToken;
    emit refreshTokenChanged();
}

bool LocalUserAuthentication::isAuthenticated() const
{
    return isAuthenticated_;
}
}
