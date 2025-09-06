#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "abstractsession.h"
#include "session.h"
#include <QProperty>
#include <QQmlEngine>

namespace Authentication
{

class AbstractKeyChain;

namespace Business { class TokenManager; }

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
    Q_PROPERTY(QString accessToken  BINDABLE bindableAccessToken FINAL)
    Q_PROPERTY(Authentication::Session currentSession READ currentSession BINDABLE bindableCurrentSession FINAL)
    QML_ELEMENT
    QML_SINGLETON
public:
    explicit SessionManager(QObject *parent = nullptr);

    // AbstractSession interface
    bool isAuthenticated() const override;
    QBindable<bool> bindableIsAuthenticated() const;

    QString accessToken() const;
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

signals:
    void errorOccurred(const QString &errorString);

    void storedSessionChecked(bool found);

    void authenticatedChanged();

private:
    QProperty<QString> accessToken_;
    QProperty<QString> refreshToken_;
    QPointer<Business::TokenManager> tokenManager_;
    QPointer<AbstractKeyChain> keychain_;
    QProperty<Session> currentSession_;
    QPropertyNotifier tokenConn_;
    Q_OBJECT_BINDABLE_PROPERTY(Authentication::SessionManager, bool, authenticated_, &SessionManager::authenticatedChanged)
};

} // namespace Authentication

#endif // SESSIONMANAGER_H
