#include "tokenmanager.h"
#include "business/itoken.h"
#include "irefreshable.h"
#include "refresher.h"
#include "tokenstore.h"
#include <QJsonDocument>
#include <QTimer>
#include <QJsonObject>

using namespace Qt::StringLiterals;

static constexpr auto ACCESS_TOKEN_LIFESPAN{840'000};

namespace Authentication::Business
{
TokenManager *TokenManager::instance_{nullptr};

TokenManager::TokenManager(QObject *parent)
    : QObject{parent}
    , refreshToken_{}
    , timer_{new QTimer(this)}
    , errorString_{}
    , store_{new Data::TokenStore{this}}
    , refresher_{new Data::Refresher(this)}
    , token_{}
    , refreshing_{true}
{
    refreshToken_.setBinding([this] { return store_->bindableRefreshToken().value(); });
    store_->restore();

    timer_->setInterval(ACCESS_TOKEN_LIFESPAN);
    timer_->start();
    connect(timer_, &QTimer::timeout, this, &TokenManager::timeout);
}

void TokenManager::refreshAccessToken()
{
    setRefreshToken(store_->refresh());
    refresher_->setRefreshToken(refreshToken_);

    const auto optionalReply{refresher_->refresh()};
    if (!optionalReply)
    {
        setErrorString("The refresh token is not present."_L1);
        qDebug() << "TokenManager::refreshAccessToken: " << errorString_;
        emit errorOccurred();
        return;
    }

    reply_ = *optionalReply;

    connect(reply_, &QNetworkReply::finished, this, &TokenManager::replyReady);
}

void TokenManager::setToken(IToken *newToken)
{
    token_ = newToken;
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

Store::IStore *TokenManager::store() const
{
    return store_;
}

void TokenManager::setStore(Store::IStore *newStore)
{
    if (store_ == newStore)
        return;
    store_ = newStore;
    emit storeChanged();
}

bool TokenManager::refreshing() const
{
    return refreshing_;
}

void TokenManager::setRefreshing(bool newRefreshing)
{
    if (refreshing_ == newRefreshing)
        return;
    refreshing_ = newRefreshing;
    emit refreshingChanged();
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

QBindable<QString> TokenManager::bindableRefreshToken() const
{
    return &refreshToken_;
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
    refreshToken().clear();
    QSettings settings{};
    settings.remove("Credentails");
}

void TokenManager::stopRefreshing()
{
    if (!timer_->isActive())
        return;

    timer_->stop();
    setRefreshing(false);
}

void TokenManager::resumeRefreshing()
{
    if (timer_->isActive())
        return;

    refreshAccessToken();
    timer_->start();
    setRefreshing(true);
}

void TokenManager::replyReady()
{
    const auto data{reply_->readAll()};
    const auto obj{QJsonDocument::fromJson(data).object()};

    if (reply_->error() != QNetworkReply::NoError)
    {
        setErrorString(reply_->errorString());
        qDebug() << "TokenManager::replyReady: " << errorString_;
        emit errorOccurred();
    }

    const QString newToken{obj["access_token"_L1].toString()};
    store_->save(data);

    if (token_)
        token_->setAccessToken(newToken);
    else
        qWarning() << "TokenManager::replyReady: Token not set.";

    setAccessToken(newToken);

    qDebug() << "TokenManager: access token has refresh";

    reply_->deleteLater();
}

void TokenManager::timeout()
{
    refreshAccessToken();
}
}
