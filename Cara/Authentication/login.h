#ifndef BUSINESS_LOGIN_H
#define BUSINESS_LOGIN_H

#include "istore.h"
#include "business/authentication.h"
#include <QObject>
#include <QQmlEngine>
#include <QByteArray>

namespace Authentication::Data { class TokenStore; }
namespace Authentication::Business
{
class Login : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(Status status READ status WRITE setStatus NOTIFY statusChanged FINAL)
    Q_PROPERTY(AuthenticationMode mode READ mode WRITE setMode NOTIFY modeChanged FINAL)
    Q_PROPERTY(Authentication *authenticator READ authenticator WRITE setAuthenticator NOTIFY authenticatorChanged FINAL REQUIRED)
    Q_PROPERTY(QQmlListProperty<Store::IStore> stores READ stores CONSTANT FINAL)
public:
    enum class Status
    {
        Authenticated,
        UnAuthenticated,
        InvalidCredentials,
        Busy,
        Error
    };
    Q_ENUM(Status)

    enum class AuthenticationMode
    {
        Undefined,
        RefreshToken,
        LoginCredentials
    };
    Q_ENUM(AuthenticationMode)

    explicit Login(QObject *parent = nullptr);

    Status status() const;
    void setStatus(Status newStatus);

    AuthenticationMode mode() const;
    void setMode(AuthenticationMode newMode);

    Authentication *authenticator() const;
    void setAuthenticator(Authentication *newAuthenticator);

    QQmlListProperty<Store::IStore> stores();

signals:
    void authenticated(const QByteArray &data);
    void errorOccurred(const QString &errorString);

    void statusChanged();

    void modeChanged();

    void authenticatorChanged();

public slots:
    void authenticate();

private slots:
    void userAuthenticated(const QByteArray &data);
    void authenticatorConnection();
    void authenticatorDisconnection();

private:
    Status status_;
    AuthenticationMode mode_;
    Authentication *authenticator_;
    QList<Store::IStore *> stores_;
    QList<QMetaObject::Connection> connections_;
};
}

#endif // BUSINESS_LOGIN_H
