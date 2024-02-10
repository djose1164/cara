#include "login.h"
#include "tokenmanager.h"
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
    connect(this, &Login::authenticated, this, &Login::userAuthenticated);
}

void Login::authenticate()
{
    authenticator_->authenticate();
}

void Login::userAuthenticated(const QByteArray &data)
{
    qDebug() << "Login::userAuthenticated: Saving...";

    for (const auto &store : std::as_const(stores_))
        store->save(data);
}

void Login::authConnections() const
{
    connect(authenticator_, &Authentication::errorOccurred,
            this, &Login::errorOccurred);
    connect(authenticator_, &Authentication::succeeded,
            this, &Login::authenticated);
    connect(authenticator_, &Business::Authentication::tokenReady, this, [&](const QString &token) {
        TokenManager::instance()->setAccessToken("Bearer "_L1 + token);
        Utils::NetworkRequestSingleton::instance().api()->setBearerToken(token.toUtf8());
    });
}

void Login::storeConnections(const Store::IStore *store) const
{
    connect(store, &Store::IStore::saved,
            this, &Login::dataSaved);
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
    authConnections();
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
    authConnections();
    for (const auto &store : std::as_const(stores_))
        storeConnections(store);
}
}
