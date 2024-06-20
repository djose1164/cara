#ifndef BUSINESS_LOGIN_H
#define BUSINESS_LOGIN_H

#include "business/authenticator.h"
#include "istore.h"
#include "tokenmanager.h"
#include <QObject>
#include <QQmlEngine>
#include <QByteArray>
#include <QProperty>

namespace Network { class IToken; };

namespace Authentication::Business
{

class Login : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Status status READ status WRITE setStatus NOTIFY statusChanged FINAL)
    Q_PROPERTY(AuthenticationMode mode READ mode WRITE setMode NOTIFY modeChanged FINAL)
    Q_PROPERTY(Authenticator *authenticator READ authenticator WRITE setAuthenticator NOTIFY authenticatorChanged FINAL REQUIRED)
    Q_PROPERTY(Store::IStore *store READ store WRITE setStore FINAL)
    Q_PROPERTY(Authenticator::Error error BINDABLE bindableError)
    Q_PROPERTY(QString errorString BINDABLE bindableErrorString)
    Q_PROPERTY(TokenManager *tokenManager READ tokenManager WRITE setTokenManager NOTIFY tokenManagerChanged FINAL)
    QML_ELEMENT
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

    Authenticator *authenticator() const;
    void setAuthenticator(Authenticator *newAuthenticator);

    Store::IStore *store();
    void setStore(Store::IStore *newStore);

    QBindable<Authenticator::Error> bindableError() const;

    QBindable<QString> bindableErrorString() const;

    TokenManager *tokenManager() const;
    void setTokenManager(TokenManager *newTokenManager);

signals:
    void authenticated(const QByteArray &data);
    void errorOccurred(const QString &errorString);

    void statusChanged();

    void modeChanged();

    void authenticatorChanged();

    void tokenManagerChanged();

public slots:
    void authenticate();

private slots:
    void userAuthenticated(const QByteArray &data);
    void authenticatorConnection();
    void authenticatorDisconnection();
    void authenticationFailed(Authenticator::Error error, const QString &errorString);

private:
    Status status_;
    AuthenticationMode mode_;
    Authenticator *authenticator_;
    Store::IStore *store_;
    QList<QMetaObject::Connection> connections_;
    QProperty<Authenticator::Error> error_;
    QProperty<QString> errorString_;
    Network::IToken *token_;
    TokenManager *tokenManager_;
};
}

#endif // BUSINESS_LOGIN_H
