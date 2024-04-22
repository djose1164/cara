#include "login.h"
#include "networkrequestsingleton.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

using namespace Qt::StringLiterals;

namespace Authentication::Business
{
Login::Login(QObject *parent)
    : QObject{parent}
    , status_{Status::UnAuthenticated}
    , mode_{AuthenticationMode::Undefined}
    , authenticator_{}
    , stores_{}
{
    // connect(this, &Login::authenticated, this, &Login::userAuthenticated);
}

void Login::authenticate()
{
    authenticator_->authenticate();
}

void Login::userAuthenticated(const QByteArray &data)
{
    qDebug() << "\nLogin::userAuthenticated: Saving...";

    for (const auto &store : std::as_const(stores_))
        store->save(data);

    emit authenticated(data);
}

QQmlListProperty<Store::IStore> Login::stores()
{
    return {this, &stores_};
}

Authentication *Login::authenticator() const
{
    return authenticator_;
}

void Login::setAuthenticator(Authentication *newAuthenticator)
{
    if (authenticator_ == newAuthenticator)
        return;
    authenticator_ = newAuthenticator;
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

void Login::classBegin()
{
}

void Login::componentComplete()
{
    connect(authenticator_, &Authentication::errorOccurred,
            this, &Login::errorOccurred);
    connect(authenticator_, &Authentication::succeeded,
            this, &Login::userAuthenticated);
    connect(authenticator_, &Business::Authentication::tokenReady, this, [&](const QString &token) {
        NetworkRequestSingleton::instance().api()->setBearerToken(token.toUtf8());
    });
}
}
