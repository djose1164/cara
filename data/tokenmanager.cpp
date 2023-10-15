#include "tokenmanager.h"
#include "refresher.h"
#include <QJsonDocument>
#include <QTimer>
#include <QJsonObject>

namespace DataLayer {
TokenManager *TokenManager::instance_{nullptr};

TokenManager::TokenManager(QObject *parent)
    : QObject{parent}
    , accessToken_{}
    , refreshToken_{}
    , refresher_{new DataLayer::Refresher(this)}
{
    auto refresher{dynamic_cast<DataLayer::Refresher *>(refresher_)};
    connect(refresher, &DataLayer::Refresher::accessTokenRefreshed, this, [&](const QByteArray &data) {
        qDebug() << "TokenManager: access token has refresh";
        const QString token{
            QJsonDocument::fromJson(data)
                .object()["access_token"]
                .toString()
        };
        setAccessToken("Bearer " + token);
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
}
}
