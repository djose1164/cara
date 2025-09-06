#include "localuserauthentication.h"
#include "networkrequestsingleton.h"
#include <QSettings>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto accessTokenField{"access_token"_L1};
constexpr auto userField{"user"_L1};
}

namespace Authentication::Service {
LocalUserAuthentication::LocalUserAuthentication(QObject *parent)
    : Business::Authenticator{parent}
    , refreshToken_{}
    , isAuthenticated_{}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{}
    , path_{"/users/refresh"_L1}
{
}

void LocalUserAuthentication::authenticate()
{
    qInfo() << "Authenticationg user through LocalUserAuthentication...";
    if (refreshToken_.isEmpty())
    {
        constexpr auto errorString{"refreshToken is empty."_L1};
        qCritical() << "LocalUserAuthentication::authenticate: " << errorString;
        emit errorOccurred(Authenticator::Error::EmptyToken, errorString);
        return;
    }

    QUrlQuery query{{"include_user"_L1, "true"_L1}};
    const auto api{Network::NetworkRequestSingleton::instance().api()};

    QNetworkRequest request{api->createRequest(path_, query)}; request.setRawHeader("Authorization"_ba, "Bearer "_ba + refreshToken_.toUtf8());

    reply_ = manager_->get(request);

    connect(reply_, &QNetworkReply::finished, this, [&] {
        qDebug() << "LocalUserAuthentication::authenticate: handling response";
        buffer_ = reply_->readAll();

        const auto replyError{reply_->error()};
        if (replyError == QNetworkReply::AuthenticationRequiredError)
        {
            emit tokenExpired();
            return;
        }
        else if (replyError != QNetworkReply::NoError)
        {
            emit errorOccurred(Authenticator::Error::Unknown,
                               reply_->errorString());
            return;
        }

        isAuthenticated_ = true;

        if (const auto json{QJsonDocument::fromJson(buffer_)};
            json.isObject() and json.object().contains(accessTokenField)
            and json.object().contains(userField))
        {
            const auto obj{json.object()};
            if (!obj["access_token"_L1].isString())
            {
                qCritical() << "LocalUserAuthentication::authenticate: token isn't a string";
                emit errorOccurred(Error::Unknown, "token isn't a string"_L1);
                return ;
            }
            const auto tokenValue{obj["access_token"_L1].toString()};
            emit tokenReady(tokenValue);
            emit succeeded(buffer_);
        }

        qDebug() << "User authenticated successfully through LocalUserAuthentication.";

        reply_->deleteLater();
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

QByteArray LocalUserAuthentication::buffer() const
{
    return buffer_;
}

void LocalUserAuthentication::setBuffer(const QByteArray &newBuffer)
{
    buffer_ = newBuffer;
}

bool LocalUserAuthentication::isAuthenticated() const
{
    return isAuthenticated_;
}
}
