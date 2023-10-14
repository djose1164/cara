#include "loginmanager.h"
#include "currentuser.h"
#include "tokenmanager.h"

namespace BusinessLayer {
LoginManager::LoginManager(QObject *parent)
    : QObject{parent}
    , status_{Status::LoggedOut}
    , auth_{nullptr}
{

}

LoginManager::Status LoginManager::status() const
{
    return status_;
}

void LoginManager::setStatus(Status status)
{
    status_ = status;
}

QBindable<LoginManager::Status> LoginManager::bindableStatus()
{
    return &status_;
}

void LoginManager::requestLogin(const QString &username, const QString &password)
{
    if (username.isEmpty() || password.isEmpty())
        return;

    auth_ = new Auth(this);

    emit loggingIn();
    auto user{new EntityLayer::UserEntity(auth_)};
    user->setUsername(username);
    user->setPassword(password);

    auth_->setUserEntity(user);
    auth_->authenticate();
    connect(auth_, &Auth::authenticated, this, [&](const QVariantMap &resp) {
        setStatus(Status::LoggedIn);

        const auto token{DataLayer::TokenManager::instance()};
        const QString accessToken{resp["access_token"].toString()};
        token->setAccessToken("Bearer " + accessToken);

        const QString refreshToken{resp["refresh_token"].toString()};
        token->setRefreshToken("Bearer " + refreshToken);

        emit loggedIn(resp);
        auth_->deleteLater();
    });
    makeConnections();
    emit loginRequested();
}

void LoginManager::requestLoginFromSettings()
{
    emit loggingIn();
    auth_ = new Auth(this);
    QSettings settings{};
    const QString token{settings.value("Credentials/refreshToken").toString()};

    auth_->authenticate(token);


    connect(auth_, &Auth::authenticatedWithToken, this, [&](const QVariantMap &data) {
        setStatus(Status::LoggedIn);
        const QString token{"Bearer " + data["access_token"].toString()};
        DataLayer::TokenManager::instance()->setAccessToken(token);
        emit loggedInWithToken(data);
        auth_->deleteLater();
    });

    makeConnections();
    emit loginRequested();
}

void LoginManager::logOut()
{
    emit loggingOut();
    setStatus(Status::LoggedOut);

    QSettings settings;
    settings.remove("Credentials");
    settings.remove("Contact");

    QQmlEngine engine;
    const auto currentUser{engine.singletonInstance<CurrentUser *>("core", "CurrentUser")};
    currentUser->clear();

    const auto manager{DataLayer::TokenManager::instance()};
    manager->clear();
    emit loggedOut();
}

bool LoginManager::hasPreviousLogin()
{
    QSettings settings{};
    const QString token{settings.value("Credentials/refreshToken").toString()};

    return !token.isEmpty();
}

void LoginManager::makeConnections()
{
    connect(auth_, &Auth::notFound, this, [&] {
        setStatus(Status::NotRegistered);
        emit notRegistered();
    });

    connect(auth_, &Auth::invalidCredentials, this, [&] {
        setStatus(Status::InvalidCredentials);
        emit invalidCredentials();
    });

    connect(auth_, &Auth::serviceUnavailable, this, &LoginManager::serviceUnavailable);
}
}
