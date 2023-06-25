#include "loginmanager.h"

using BusinessLayer::Auth, EntityLayer::UserEntity;

LoginManager::LoginManager(QObject *parent)
    : QObject{parent}
    , status_{Status::LoggedOut}
    , auth_{new BusinessLayer::Auth(this)}
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
    auto user{new EntityLayer::UserEntity(auth_)};
    user->setUsername(username);
    user->setPassword(password);

    auth_->setUserEntity(user);
    auth_->authenticate();

    emit loginRequested();

    makeConnections();
}

void LoginManager::requestLoginFromSettings()
{
    QSettings settings{};
    const QString token{settings.value("Credentials/refreshToken").toString()};

    auth_ = new Auth(this);
    auth_->authenticate(token);

    makeConnections();
}

bool LoginManager::hasPreviousLogin()
{
    QSettings settings{};
    const QString token{settings.value("Credentials/refreshToken").toString()};

    return !token.isEmpty();
}

void LoginManager::makeConnections()
{
    connect(auth_, &Auth::authenticationDone, this, [&] {
        emit authenticationDone();
        auth_->deleteLater();
    });

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
}
