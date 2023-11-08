#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H

#include "service/userservice.h"
#include "session.h"
#include <QObject>
#include <QQmlEngine>
#include <QObjectBindableProperty>

namespace BusinessLayer {
class LoginManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(Status status READ status WRITE setStatus NOTIFY statusChanged BINDABLE bindableStatus)
    Q_PROPERTY(Session *session READ session WRITE setSession NOTIFY sessionChanged FINAL)
public:
    enum class Status
    {
        LoggedIn,
        LoggedOut,
        InvalidCredentials,
        NotRegistered
    };
    Q_ENUM(Status)

    explicit LoginManager(QObject *parent = nullptr);

    Status status() const;
    void setStatus(Status status);
    QBindable<Status> bindableStatus();

    Session *session() const;
    void setSession(Session *newSession);

public slots:
    void logOut();
    void authenticated();

signals:
    void statusChanged();
    void loginRequested();

    void loggedIn();
    void loggedInWithToken();
    void loggedOut();
    void invalidCredentials();
    void notRegistered();
    void authenticationDone();

    void loggingIn();
    void loggingOut();

    void serviceUnavailable();

    void sessionChanged();

private:
    Q_OBJECT_BINDABLE_PROPERTY(LoginManager, Status, status_, &LoginManager::statusChanged);
    Service::UserService *userService_;
    Session *session_;
};
}

#endif // LOGINMANAGER_H
