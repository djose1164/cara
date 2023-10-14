#include "tokenmanager.h"
#include "refresher.h"
#include <QJsonDocument>
#include <QTimer>
#include <QJsonObject>

namespace DataLayer {
TokenManager::TokenManager(QObject *parent)
    : QObject{parent}
    , accessToken_{}
    , refreshToken_{}
    , refresher_{new DataLayer::Refresher(this)}
    , timer_{new QTimer(this)}
{
    timer_->start(840'000);
    connect(timer_, &QTimer::timeout, this, [&] {
        refresher_->refresh();
    });

    auto refresher{dynamic_cast<DataLayer::Refresher *>(refresher_)};
    connect(refresher, &DataLayer::Refresher::accessTokenRefreshed, this, [&](const QByteArray &data) {
        qDebug() << "Token refreshed: " << data;
        const QString token{
            QJsonDocument::fromJson(data)
                .object()["refresh_token"]
                .toString()
        };
        setAccessToken(token);
    });
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
    auto refresher{dynamic_cast<DataLayer::Refresher *>(refresher_)};
    refresher->setRefreshToken(refreshToken_);
    emit refreshTokenChanged();
}

TokenManager *TokenManager::instance()
{
    static TokenManager tokenManager{};
    return &tokenManager;
}

void TokenManager::clear()
{
    accessToken_.clear();
    refreshToken_.clear();
}
}
