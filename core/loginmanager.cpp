#include "loginmanager.h"
#include "tokenmanager.h"
#include "service/userserviceimpl.h"

namespace BusinessLayer {
LoginManager::LoginManager(QObject *parent)
    : QObject{parent}
    , status_{Status::LoggedOut}
    , userService_{new Service::UserServiceImpl(this)}
    , session_{}
{
    connect(this, &LoginManager::statusChanged, this, [&] {
        qDebug() << "newStatus: " << status_.value();
        if (!session_)
            return;
        if ( status_ == Status::LoggedOut)
        {
            qDebug() << "Loginmanager:: clearing session.";
            session_->clear();
        }
        else
        {
            qDebug() << "Loginmanager:: updating session.";
            session_->update();
        }
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

void LoginManager::logOut()
{
    emit loggingOut();
    setStatus(Status::LoggedOut);

    userService_->clear();

    const auto manager{DataLayer::TokenManager::instance()};
    manager->clear();
    emit loggedOut();
}
void LoginManager::authenticated()
{
    setStatus(Status::LoggedIn);
    emit loggedIn();
}

Session *LoginManager::session() const
{
    return session_;
}

void LoginManager::setSession(Session *newSession)
{
    if (session_ == newSession)
        return;
    session_ = newSession;
    emit sessionChanged();
}
}
