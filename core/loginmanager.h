#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H

#include "auth.h"
#include <QObject>
#include <QQmlEngine>
#include <QObjectBindableProperty>

namespace BusinessLayer {
class LoginManager;
}

class LoginManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(Status status READ status WRITE setStatus NOTIFY statusChanged BINDABLE bindableStatus)

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

public slots:
    void requestLogin(const QString &username, const QString &password);
    void requestLoginFromSettings();
    void logOut();
    bool hasPreviousLogin();

private slots:
    void makeConnections();

signals:
    void statusChanged();
    void loginRequested();

    void loggedIn(const QVariantMap &resp);
    void loggedInWithToken();
    void loggedOut();
    void invalidCredentials();
    void notRegistered();
    void authenticationDone();

    void loggingIn();
    void loggingOut();

    void serviceUnavailable();

private:
    Q_OBJECT_BINDABLE_PROPERTY(LoginManager, Status, status_, &LoginManager::statusChanged);

    BusinessLayer::Auth *auth_;
};

#endif // LOGINMANAGER_H
