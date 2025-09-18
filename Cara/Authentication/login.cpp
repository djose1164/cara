#include "login.h"
#include <QJsonDocument>
#include <QJsonObject>

using namespace Qt::StringLiterals;

namespace Authentication
{
Login::Login(QObject *parent)
    : QObject{parent}
    , status_{Status::UnAuthenticated}
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
        setStatus(Status::Authenticated);
        emit authenticated(session);
    }

    qDebug() << "Login::userAuthenticated...Done!";
}

void Login::authenticatorConnection()
{
    connect(authenticator_, &Authenticator::errorOccurred, this, &Login::authenticationFailed);

    connect(authenticator_, &Authenticator::authenticationSucceeded, this, &Login::userAuthenticated);

    connect(authenticator_, &Authenticator::tokenGenerated, this, [&](const QString &token) {
        Q_UNUSED(token);
        qDebug() << "Login: tokenReady";
    });
}

void Login::authenticationFailed(Authenticator::Error error, const QString &errorString)
{
    qWarning() << "Login::authenticationFailed: " << QVariant::fromValue(error);
    using enum Authenticator::Error;
    errorString_ = errorString;
    if (error == InvalidCredentials)
    {
        setStatus(Status::UnAuthenticated);
        error_ = InvalidCredentials;
    }
    emit errorOccurred(error, errorString);
}

QBindable<Authenticator::Error> Login::bindableError() const
{
    return &error_;
}

QString Login::errorString() const
{
    return errorString_;
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

    if (authenticator_)
        authenticator_->disconnect(this);
    authenticator_ = newAuthenticator;
    authenticatorConnection();
    emit authenticatorChanged();
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
