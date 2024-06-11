#include "localuserauthentication.h"
#include "networkrequestsingleton.h"
#include "tokenstore.h"
#include <QSettings>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

using namespace Qt::StringLiterals;

namespace Authentication::Service {
LocalUserAuthentication::LocalUserAuthentication(QObject *parent)
    : Business::Authenticator{parent}
    , refreshToken_{}
    , isAuthenticated_{}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{}
{
    Data::TokenStore ts{};
    ts.restore();
    setRefreshToken(ts.refresh());
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

    const auto api{NetworkRequestSingleton::instance().api()};

    QNetworkRequest request{api->createRequest("/users/refresh/"_L1)};
    request.setRawHeader("Authorization"_ba, "Bearer "_ba + refreshToken_.toUtf8());

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

        const auto resp{QJsonDocument::fromJson(buffer_).object()};
        const auto tokenValue{resp["access_token"_L1]};
        if (!tokenValue.isString())
        {
            qCritical() << "LocalUserAuthentication::authenticate: token isn't a string";
            return ;
        }
        const QString newToken{tokenValue.toString()};

        qDebug() << "User authenticated successfully through LocalUserAuthentication.";
        emit succeeded(buffer_);
        emit tokenReady(newToken);

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
