#include "loginmanager.h"

using BusinessLayer::Auth;

LoginManager::LoginManager(QObject *parent)
    : QObject{parent}
    , status_{Status::LoggedOut}
    , auth_{new BusinessLayer::Auth(this)}
{
    connect(auth_, &Auth::authenticationDone, this, &LoginManager::authenticationDone);

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

    auto user{new EntityLayer::UserEntity(auth_)};
    user->setUsername(username);
    user->setPassword(password);

    auth_->setUserEntity(user);
    auth_->authenticate();

    emit loginRequested();
}
