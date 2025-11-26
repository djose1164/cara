#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "abstractsession.h"
#include "login.h"
#include "session.h"
#include <QProperty>
#include <QQmlEngine>

namespace Authentication
{

class AbstractKeyChain;
class TokenManager;

/**
 * @brief The SessionManager class manages the current session of the user and
 * the access and refresh token. After the singlenton is created by QmlEngine,
 * the SessionManager will check wether there's or not a stored session, that is,
 * a previously stored refresh token.
 */
class SessionManager : public AbstractSession
{
    Q_OBJECT
    Q_PROPERTY(bool authenticated READ isAuthenticated NOTIFY authenticatedChanged BINDABLE bindableIsAuthenticated FINAL)
    Q_PROPERTY(QString accessToken READ accessToken NOTIFY accessTokenChanged BINDABLE bindableAccessToken FINAL)
    Q_PROPERTY(Authentication::Session currentSession READ currentSession BINDABLE bindableCurrentSession FINAL)
    QML_ELEMENT
    QML_SINGLETON
public:
    enum class Error
    {
        AuthenticationFailed,
        ConnectionLost,
        SessionExpired,
        TokenMissing,
        KeychainError,
        Unknown
    };
    Q_ENUM(Error)

    explicit SessionManager(QObject *parent = nullptr);

    // AbstractSession interface
    bool isAuthenticated() const override;
    QBindable<bool> bindableIsAuthenticated() const;

    QString accessToken() const;
    void setAccessToken(const QString &newAccessToken);
    QBindable<QString> bindableAccessToken();

    QBindable<Session> bindableCurrentSession();
    Session currentSession() const;
    void setCurrentSession(const Session &newSession);

    QString refreshToken() const;

public slots:
    void clearSession() override;

    void startSession(const Authentication::Session &session) override;

    /**
     * @brief checkStoredSession verify whether the user has a stored session,
     * that is, a refresh token was previously stored.
     */
    void checkStoredSession();

    /**
     * @brief authenticateRefreshToken tries to authenticate the refresh token
     * stored in the keychain. If there's no token, errorOccurred would be emitted.
     */
    void authenticateRefreshToken();

    /**
     * @brief loadOfflineSession tries to load and start any offline/stored session.
     * By offline/stored session we mean any user info saved locally after
     * the last successful login. This' useful to use the app in offline mode.
     */
    void loadOfflineSession();

signals:
    void errorOccurred(Error error, const QString &errorString);

    void storedSessionChecked(bool found);

    void authenticatedChanged();

    void dataFetched(const QJsonObject &json);

    void accessTokenChanged();

private slots:
    void handleLoginError(Login::Error error, const QString &errorString);
    void onErrorOccurred(Error error, const QString &errorString);

private:
    QProperty<QString> accessToken_;
    QProperty<QString> refreshToken_;
    QPointer<TokenManager> tokenManager_;
    QPointer<AbstractKeyChain> keychain_;
    QProperty<Session> currentSession_;
    QPropertyNotifier tokenConn_;
    Q_OBJECT_BINDABLE_PROPERTY(Authentication::SessionManager, bool, authenticated_, &SessionManager::authenticatedChanged)
};

} // namespace Authentication

#endif // SESSIONMANAGER_H
