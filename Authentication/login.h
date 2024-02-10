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
class Login : public QObject, public QQmlParserStatus
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
    
    Store::IStore *store() const;
    void setStore(Store::IStore *newUserStore);

    AuthenticationMode mode() const;
    void setMode(AuthenticationMode newMode);

    void classBegin() override;
    void componentComplete() override;

    Authentication *authenticator() const;
    void setAuthenticator(Authentication *newAuthenticator);

    Data::TokenStore *tokenStore() const;

    void setTokenStore(Data::TokenStore *newTokenStore);

    QQmlListProperty<Store::IStore> stores();

signals:
    void authenticated(const QByteArray &data);
    void errorOccurred();

    void statusChanged();

    void dataSaved();

    void storeChanged();

    void modeChanged();

    void authenticatorChanged();

    void tokenStoreChanged();

public slots:
    void authenticate();

private slots:
    void userAuthenticated(const QByteArray &data);

private:
    void authConnections() const;
    void storeConnections(const Store::IStore *store) const;

private:
    Status status_;
    AuthenticationMode mode_;
    Authentication *authenticator_;
    QList<Store::IStore *> stores_;
};
}

#endif // BUSINESS_LOGIN_H
