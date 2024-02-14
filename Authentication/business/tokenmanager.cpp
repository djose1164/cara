#include "tokenmanager.h"
#include "business/itoken.h"
#include "irefreshable.h"
#include "refresher.h"
#include "tokennotpresent.h"
#include "networkrequestsingleton.h"
#include "tokenstore.h"
#include <QJsonDocument>
#include <QTimer>
#include <QJsonObject>

using namespace Qt::StringLiterals;

namespace Authentication::Business
{
TokenManager *TokenManager::instance_{nullptr};

TokenManager::TokenManager(QObject *parent)
    : QObject{parent}
    , accessToken_{}
    , refreshToken_{}
    , timer_{new QTimer(this)}
    , errorString_{}
    , tokenStore_{new Data::TokenStore(this)}
    , refresher_{new Data::Refresher(this)}
    , token_{&Utils::NetworkRequestSingleton::instance()}
{
    timer_->start(840'000);
    connect(timer_, &QTimer::timeout, this, &TokenManager::timeout);
}

QString TokenManager::errorString() const
{
    return errorString_;
}

void TokenManager::setErrorString(const QString &newErrorString)
{
    errorString_ = newErrorString;
}

IRefreshable *TokenManager::refresher() const
{
    return refresher_;
}

void TokenManager::setRefresher(IRefreshable *newRefresher)
{
    refresher_ = newRefresher;
}

QString TokenManager::accessToken() const
{
    return accessToken_;
}

void TokenManager::setAccessToken(const QString &newAccessToken)
{
    if (accessToken_ == newAccessToken)
        return;
    accessToken_ = newAccessToken;
    emit accessTokenChanged();
}

QString TokenManager::refreshToken() const
{
    return refreshToken_;
}

void TokenManager::setRefreshToken(const QString &newRefreshToken)
{
    if (refreshToken_ == newRefreshToken)
        return;
    refreshToken_ = newRefreshToken;
    emit refreshTokenChanged();
}

TokenManager *TokenManager::create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
    return instance();
}

TokenManager *TokenManager::instance()
{
    if (instance_)
        return instance_;

    instance_ = new TokenManager;
    return instance_;
}

void TokenManager::clear()
{
    accessToken_.clear();
    refreshToken_.clear();
    QSettings settings{};
    settings.remove("Credentails");
}

void TokenManager::replyReady()
{
    const auto data{reply_->readAll()};
    const auto obj{QJsonDocument::fromJson(data).object()};

    if (reply_->error() != QNetworkReply::NoError)
    {
        setErrorString(reply_->errorString());
        emit errorOccurred();
    }

    const QString newToken{obj["access_token"_L1].toString()};
    // tokenStore_->save(data); // FIXME: It's causing a double dispatch of onSaved handler in main.qml
    token_->setAccessToken(newToken);
    setAccessToken("Bearer "_L1 + newToken);

    qDebug() << "TokenManager: access token has refresh";

    reply_->deleteLater();
}

void TokenManager::timeout()
{
    try
    {
        Data::TokenStore ts{};
        ts.restore();
        setRefreshToken(ts.refresh());
        refresher_->setRefreshToken(refreshToken_);

        reply_ = refresher_->refresh();

        connect(reply_, &QNetworkReply::finished, this, &TokenManager::replyReady);
    }
    catch(const Business::TokenNotPresent &err)
    {
        // TODO: The LoginManager should be aware of this to ask the user to log in again.
        setErrorString(err.what());
        emit errorOccurred();
    }
}
}
