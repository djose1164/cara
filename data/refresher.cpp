#include "refresher.h"
#include "tokenmanager.h"
#include <QJsonDocument>
#include <QNetworkReply>
#include <QJsonObject>

namespace DataLayer {
Refresher::Refresher(QObject *parent)
    : QObject{parent}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{nullptr}
    , refreshToken_{}
    , timer_{new QTimer(this)}
    , settings_{}
{
    timer_->start(840'000);
    connect(timer_, &QTimer::timeout, this, [&] {
        refresh();
    });
}

void Refresher::refresh()
{
    const QString token{settings_.value("User/refreshToken").toString()};
    setRefreshToken(token);
    if (refreshToken_.isEmpty())
    {
        qWarning() << "Refresher::refresh: Refresh token is empty.";
        return;
    }

    const QUrl url{settings_.value("base").toString() + "/users/refresh/"};
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", refreshToken_.toUtf8());

    reply_ = manager_->get(request);

    connect(reply_, &QNetworkReply::finished, this, [&] {
        const auto data{reply_->readAll()};
        const auto obj{QJsonDocument::fromJson(data).object()};

        if (reply_->error() == QNetworkReply::NoError)
            emit accessTokenRefreshed("Bearer " + obj["access_token"].toString());
        else
            emit errorOccurred(data);
    });
}

QString Refresher::refreshToken() const
{
    return refreshToken_;
}

void Refresher::setRefreshToken(const QString &newRefreshToken)
{
    if (refreshToken_ == newRefreshToken)
        return;
    refreshToken_ = newRefreshToken;
    emit refreshTokenChanged();
}
}
