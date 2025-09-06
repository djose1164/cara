#include "tokenmanager.h"
#include "abstractaccesstokenrefresher.h"
#include "accesstokenrefresher.h"
#include <QJsonDocument>
#include <QTimer>
#include <QJsonObject>

using namespace Qt::StringLiterals;

static constexpr auto ACCESS_TOKEN_LIFESPAN{840'000};

namespace Authentication::Business
{

TokenManager::TokenManager(QObject *parent)
    : QObject{parent}
    , refreshToken_{}
    , timer_{}
    , errorString_{}
    , refresher_{}
    , refreshing_{true}
{
}

TokenManager::~TokenManager()
{
    stop();
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

AbstractAccessTokenRefresher *TokenManager::refresher() const
{
    return refresher_;
}

void TokenManager::setRefresher(AbstractAccessTokenRefresher *newRefresher)
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

void TokenManager::clear()
{
    QSettings settings{};
    settings.remove("Credentails");
}

void TokenManager::stopRefreshing()
{
    if (not timer_ or not timer_->isActive())
        return;

    timer_->stop();
    setRefreshing(false);
}

void TokenManager::resumeRefreshing()
{
    if (not timer_ or timer_->isActive())
        return;

    timer_->start();
    setRefreshing(true);
}

void TokenManager::start()
{
    stop();

    timer_ = new QTimer(this);
    timer_->setInterval(ACCESS_TOKEN_LIFESPAN);
    timer_->start();
    connect(timer_, &QTimer::timeout, this, &TokenManager::timeout);
}

void TokenManager::stop()
{
    if (timer_)
    {
        timer_->deleteLater();
        timer_ = nullptr;
    }
}

void TokenManager::timeout()
{
    Q_ASSERT(refreshToken().size() > 0);
    if (refreshToken().isEmpty())
    {
        setErrorString("The refresh token is not present."_L1);
        qDebug() << "TokenManager::refreshAccessToken: " << errorString_;
        emit errorOccurred(errorString());
        return;
    }

    refresher_ = new Data::AccessTokenRefresher(this);
    refresher_->setRefreshToken(refreshToken_);

    refresher_->refresh();

    connect(refresher_,
            &Authentication::Data::AccessTokenRefresher::errorOccurred,
            this,
            &TokenManager::errorOccurred);
    connect(refresher_, &Authentication::Data::AccessTokenRefresher::refreshed, this, [this](const auto &token) {
        setAccessToken(token);
        refresher_->deleteLater();
        refresher_ = nullptr;
        qDebug() << "TokenManager: access token has refresh";
    });
}

} // namespace Authentication::Business
