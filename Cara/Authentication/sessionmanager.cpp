#include "sessionmanager.h"
#include "data/localuserstore.h"
#ifdef Q_OS_ANDROID
#include "androidkeychain.h"
#else
#include "keychainimpl.h"
#endif
#include "localuserauthentication.h"
#include "login.h"
#include "networkrequestsingleton.h"
#include "tokenmanager.h"

#include <QNetworkInformation>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto refreshKey{"refreshToken"_L1};
}

namespace Authentication
{

SessionManager::SessionManager(QObject *parent)
    : Authentication::AbstractSession{parent}
    , accessToken_{}
    , refreshToken_{}
    , tokenManager_{}
#ifdef Q_OS_ANDROID
    , keychain_{new AndroidKeyChain(this)}
#else
    , keychain_{new KeyChainImpl(this)}
#endif
    , currentSession_{}
    , tokenConn_{}
    , authenticated_{}
{
    connect(this, &SessionManager::storedSessionChecked, this, [this](bool found) {
        const auto msg = found ? "session found!"_L1 : "there's no stored session!"_L1;
        qDebug() << "SessionManager: " << msg;
        qDebug() << "SessionManager: " << refreshToken_;
        qDebug() << "SessionManager::checkStoredSession: checking stored session...Done!";
    });

    connect(keychain_, &AbstractKeyChain::keyStored, this, [] {
        qDebug() << "SessionManager::startSession: refresh token stored!";
    });

    connect(keychain_,
            &AbstractKeyChain::errorOccurred,
            this,
            [this](AbstractKeyChain::Error error, const QString &errstr) {
                switch (error)
                {
                    using enum AbstractKeyChain::Error;
                case ReadFailed:
                    emit storedSessionChecked(false);
                    break;
                case WriteFailed:
                case DeleteFailed:
                    break;
                }
                emit errorOccurred(Error::KeychainError, errstr);
            });

    connect(keychain_,
            &AbstractKeyChain::keyRestored,
            this,
            [this](const QString &key, const QString &value) {
                if (key != refreshKey)
                    return;
                refreshToken_ = value;
                emit storedSessionChecked(not value.trimmed().isEmpty());
            });

    tokenConn_ = accessToken_.addNotifier(
        [this] { Network::NetworkRequestSingleton::instance().setAccessToken(accessToken_); });
}

bool SessionManager::isAuthenticated() const
{
    return authenticated_;
}

QBindable<bool> SessionManager::bindableIsAuthenticated() const
{
    return &authenticated_;
}

QString SessionManager::accessToken() const
{
    return accessToken_;
}

void SessionManager::setAccessToken(const QString &newAccessToken)
{
    if (accessToken_ == newAccessToken)
        return;
    accessToken_ = newAccessToken;
    emit accessTokenChanged();
}

QBindable<QString> SessionManager::bindableAccessToken()
{
    return &accessToken_;
}

QBindable<Session> SessionManager::bindableCurrentSession()
{
    return &currentSession_;
}

Session SessionManager::currentSession() const
{
    return currentSession_;
}

void SessionManager::setCurrentSession(const Session &newSession)
{
    currentSession_ = newSession;
}

QString SessionManager::refreshToken() const
{
    return refreshToken_;
}

void SessionManager::clearSession()
{
    keychain_->deleteKey(refreshKey);
    if (tokenManager_)
        tokenManager_->deleteLater();
    authenticated_ = false;
    emit sessionCleared();
}

void SessionManager::startSession(const Authentication::Session &session)
{
    qDebug() << "SessionManager::startSession: starting session...";

    currentSession_ = session;

    setAccessToken(session.accessToken());

    refreshToken_ = session.refreshToken();
    if (tokenManager_)
        tokenManager_->deleteLater();

    tokenManager_ = new TokenManager(this);
    tokenManager_->setRefreshToken(refreshToken_);
    if (accessToken().isEmpty())
    {
        connect(tokenManager_, &TokenManager::accessTokenChanged, this, [&] {
            setAccessToken(tokenManager_->accessToken());
            tokenManager_->deleteLater();
        });
    }

    qDebug() << "session.accessToken(): " << accessToken();
    if (not session.refreshToken().isEmpty())
        keychain_->writeKey(refreshKey, session.refreshToken());

    authenticated_ = true;
    emit sessionStarted();
    qDebug() << "SessionManager::startSession: starting session...Done!";
}

void SessionManager::checkStoredSession()
{
    qDebug() << "SessionManager::checkStoredSession: checking stored session...";
    keychain_->readKey(refreshKey);
}

void SessionManager::authenticateRefreshToken()
{
    qDebug() << "SessionManager::authenticateRefreshToken: authenticating...";
    if (refreshToken().isEmpty())
    {
        const auto errmsg{"Cannot authenticate an empty token!"_L1};
        qWarning() << "SessionManager::authenticateRefreshToken: " << errmsg;
        emit errorOccurred(Error::TokenMissing, errmsg);
        return;
    }

    auto *login{new Login(this)};
    auto *lsa{new Service::LocalUserAuthentication(login)};
    lsa->setRefreshToken(refreshToken_);

    login->setAuthenticator(lsa);
    login->authenticate();

    connect(login, &Login::dataFetched, this, &SessionManager::dataFetched);
    connect(login, &Login::authenticated, this, [=, this](const auto &session) {
        startSession(session);
        login->deleteLater();
        qDebug() << "SessionManager::authenticateRefreshToken: authenticating...Done!";
    });

    connect(login, &Login::errorOccurred, this, [=, this](Login::Error error, const QString &errstr) {
        login->deleteLater();
        handleLoginError(error, errstr);
        // Something is wrong with our refresh token, so we delete it.
        if (error == Login::Error::InvalidState)
        {
            keychain_->deleteKey(refreshKey);
            qWarning() << "SessionManager::authenticateRefreshToken: refresh Key deleted due to: "
                       << errstr;
        }
    });
}

void SessionManager::loadOfflineSession()
{
    qDebug() << "SessionManager::loadOfflineSession: loading session...";
    User::Data::LocalUserStore lus;
    lus.restore();

    Session s;
    s.setUser(lus.user());
    s.setAccessToken(accessToken_);
    s.setRefreshToken(refreshToken_);
    qDebug() << "SessionManager::loadOfflineSession: starting session...";
    startSession(s);
}

void SessionManager::handleLoginError(Login::Error error, const QString &errorString)
{
    qWarning() << "SessionManager::handleLoginError: " << errorString;
    switch (error)
    {
        using enum Authenticator::Error;
    case InvalidCredentials:
    case InvalidState:
        emit errorOccurred(Error::AuthenticationFailed, errorString);
        break;
    case RefreshTokenExpired:
        emit errorOccurred(Error::SessionExpired, errorString);
        break;
    case NetworkUnavailable:
        emit errorOccurred(Error::ConnectionLost, errorString);
        break;
    case Unknown:
        emit errorOccurred(Error::ConnectionLost, errorString);
        break;
    }
}

void SessionManager::onErrorOccurred(Error error, const QString &errorString)
{
    switch (error)
    {
    case Error::AuthenticationFailed:
    case Error::ConnectionLost:
        break;
    case Error::SessionExpired:

        break;
    case Error::Unknown:
        break;
    }
}

} // namespace Authentication
