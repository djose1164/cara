#include "loginmanager.h"

using BusinessLayer::Auth, EntityLayer::UserEntity;

LoginManager::LoginManager(QObject *parent)
    : QObject{parent}
    , status_{Status::LoggedOut}
    , auth_{new Auth(this)}
{
    connect(auth_, &BusinessLayer::Auth::authenticated, this, [&](const QVariantMap &resp) {
        setStatus(Status::LoggedIn);
        emit loggedIn(resp);
    });

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

    emit loggingIn();
    auto user{new EntityLayer::UserEntity(auth_)};
    user->setUsername(username);
    user->setPassword(password);

    auth_->setUserEntity(user);
    auth_->authenticate();

    emit loginRequested();
}

void LoginManager::requestLoginFromSettings()
{
    emit loggingIn();
    QSettings settings{};
    const QString token{settings.value("Credentials/refreshToken").toString()};

    auth_->authenticate(token);
}

void LoginManager::logOut()
{
    emit loggingOut();
    setStatus(Status::LoggedOut);

    QSettings settings;
    settings.remove("Credentials");
    settings.remove("Contact");
    emit loggedOut();
}

bool LoginManager::hasPreviousLogin()
{
    QSettings settings{};
    const QString token{settings.value("Credentials/refreshToken").toString()};
    auth_->reset();

    return !token.isEmpty();
}

void LoginManager::makeConnections()
{

}
