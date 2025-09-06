#include "login.h"
#include "networkrequestsingleton.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto accessTokenField{"access_token"_L1};
constexpr auto refreshTokenField{"refresh_token"_L1};
constexpr auto userField{"user"_L1};
}

namespace Authentication::Business
{
Login::Login(QObject *parent)
    : QObject{parent}
    , status_{Status::UnAuthenticated}
    , mode_{AuthenticationMode::Undefined}
    , authenticator_{}
{
}

void Login::authenticate()
{
    if (!authenticator_)
        return;
    setStatus(Status::Busy);
    authenticator_->authenticate();
}

void Login::userAuthenticated(const QByteArray &data)
{
    qDebug() << "Login::userAuthenticated...";
    setStatus(Status::Authenticated);

    if (const auto json{QJsonDocument::fromJson(data)};
        json.isObject())
    {     
        Session session {Session::fromJson(json.object())};
        emit authenticated(session);
    }

    qDebug() << "Login::userAuthenticated...Done!";
}

void Login::authenticatorConnection()
{
    connections_.append(
        connect(authenticator_,
                &Authenticator::errorOccurred,
                this,
                &Login::authenticationFailed
        )
    );
    connections_.append(
        connect(authenticator_, &Authenticator::succeeded,
            this, &Login::userAuthenticated
        )
    );
    connections_.append(
        connect(authenticator_,
            &Business::Authenticator::tokenReady,
            this, [&](const QString &token)
            {
                qDebug() << "Login: tokenReady";
            }
        )
    );
}

void Login::authenticatorDisconnection()
{
    for (const auto &connection : connections_)
    {
        disconnect(connection);
    }
    connections_.clear();
}

void Login::authenticationFailed(Authenticator::Error error, const QString &errorString)
{
    qWarning() << "Login::authenticationFailed: " << QVariant::fromValue(error);
    using enum Authenticator::Error;
    errorString_ = errorString;
    if (error == InvalidCredentials)
    {
        setStatus(Status::InvalidCredentials);
        error_ = InvalidCredentials;
    }
    emit errorOccurred(errorString);
}

QBindable<Authenticator::Error> Login::bindableError() const
{
    return &error_;
}

QBindable<QString> Login::bindableErrorString() const
{
    return &errorString_;
}

Authenticator *Login::authenticator() const
{
    return authenticator_;
}

void Login::setAuthenticator(Authenticator *newAuthenticator)
{
    if (authenticator_ == newAuthenticator)
        return;
    authenticatorDisconnection();
    authenticator_ = newAuthenticator;
    authenticatorConnection();
    emit authenticatorChanged();
}

Login::AuthenticationMode Login::mode() const
{
    return mode_;
}

void Login::setMode(AuthenticationMode newMode)
{
    if (mode_ == newMode)
        return;
    mode_ = newMode;
    emit modeChanged();
}

Login::Status Login::status() const
{
    return status_;
}

void Login::setStatus(Status newStatus)
{
    if (status_ == newStatus)
        return;
    status_ = newStatus;
    emit statusChanged();
}

}
