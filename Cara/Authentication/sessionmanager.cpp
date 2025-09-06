#include "sessionmanager.h"
#include "keychainimpl.h"
#include "localuserauthentication.h"
#include "login.h"
#include "networkrequestsingleton.h"
#include "tokenmanager.h"

using namespace Qt::StringLiterals;

namespace
{
constexpr auto refreshKey{"refreshToken"_L1};
}

namespace Authentication
{

SessionManager::SessionManager(QObject *parent)
    : Authentication::AbstractSession{parent}
    , authenticated_{}
    , accessToken_{}
    , refreshToken_{}
    , tokenManager_{}
    , keychain_{new KeyChainImpl(this)}
    , currentSession_{}
    , tokenConn_{}
{
    checkStoredSession();

    connect(this, &SessionManager::storedSessionChecked, this, [](bool found) {
        const auto msg = found ? "session found!"_L1 : "there's no stored session!"_L1;
        qDebug() << "SessionManager: " << msg;
        qDebug() << "SessionManager::checkStoredSession: checking stored session...Done!";
    });

    connect(keychain_, &AbstractKeyChain::keyStored, this, [] {
        qDebug() << "SessionManager::startSession: refresh token stored!";
    });

    connect(keychain_, &AbstractKeyChain::errorOccurred, this, &SessionManager::errorOccurred);

    connect(keychain_,
            &AbstractKeyChain::keyRestored,
            this,
            [this](const QString &key, const QString &value) {
                if (key != refreshKey)
                    return;
                refreshToken_ = value;
                emit storedSessionChecked(not value.trimmed().isEmpty());
            });

    tokenConn_ = accessToken_.addNotifier([this] {
        Network::NetworkRequestSingleton::instance().setAccessToken(accessToken_);
    });
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

    bool isNewAccessTokenEmpty{session.accessToken().isEmpty()};
    if (not isNewAccessTokenEmpty)
        accessToken_ = session.accessToken();

    refreshToken_ = session.refreshToken();
    if (tokenManager_)
        tokenManager_->deleteLater();

    tokenManager_ = new Business::TokenManager(this);
    tokenManager_->setRefreshToken(refreshToken_);
    if (isNewAccessTokenEmpty)
    {
        connect(tokenManager_, &Business::TokenManager::accessTokenChanged, this, [&] {
            accessToken_ = tokenManager_->accessToken();
            tokenManager_->deleteLater();
        });
    }

    qDebug() << "session.refreshToken(): " << session.refreshToken();
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
        emit errorOccurred(errmsg);
        return;
    }

    auto *login{new Business::Login(this)};
    auto *lsa{new Service::LocalUserAuthentication(login)};
    lsa->setRefreshToken(refreshToken_);

    login->setAuthenticator(lsa);
    login->authenticate();

    connect(login, &Business::Login::authenticated, this, [=, this](const auto &session) {
        startSession(session);
        login->deleteLater();
        qDebug() << "SessionManager::authenticateRefreshToken: authenticating...Done!";
    });
}

} // namespace Authentication
