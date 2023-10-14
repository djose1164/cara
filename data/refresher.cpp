#include "refresher.h"
#include "constants.h"


namespace DataLayer {
Refresher::Refresher(QObject *parent)
    : QObject{parent}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{nullptr}
    , refreshToken_{}
    , timer_{new QTimer(this)}
{
    timer_->start(840'000);
    connect(timer_, &QTimer::timeout, this, [&] {
        refresh();
    });
}

void Refresher::refresh()
{
    if (refreshToken_.isEmpty())
    {
        qWarning() << "Refresher::refresh: Refresh token is empty.";
        return;
    }

    QQmlEngine engine;
    const auto instance{engine.singletonInstance<DataLayer::Constants *>("data", "Constants")};
    const QUrl url{instance->base() + "/users/refresh/"};

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", refreshToken_.toUtf8());

    reply_ = manager_->get(request);

    connect(reply_, &QNetworkReply::finished, this, [&] {
        const auto data{reply_->readAll()};

        if (reply_->error() == QNetworkReply::NoError)
            emit accessTokenRefreshed(data);
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
