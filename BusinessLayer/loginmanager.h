#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H

#include "rest.h"
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
        AuthFailed
    };

    explicit LoginManager(QObject *parent = nullptr);

    Status status() const;
    void setStatus(Status status);
    QBindable<Status> bindableStatus();

public slots:
    void requestLogin(const QString &username, const QString &password);

signals:
    void statusChanged();
    void loginRequested();
    void responseReady();

private:
    Q_OBJECT_BINDABLE_PROPERTY(LoginManager, LoginManager::Status, status_, &LoginManager::statusChanged);
    Rest *rest_;
};

#endif // LOGINMANAGER_H
