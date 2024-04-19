#include "refresher.h"
#include "networkrequestsingleton.h"
#include <QJsonDocument>
#include <QNetworkReply>
#include <QJsonObject>
#include <QNetworkRequestFactory>

using namespace Qt::StringLiterals;

namespace Authentication::Data{
Refresher::Refresher(QObject *parent)
    : QObject{parent}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{nullptr}
    , refreshToken_{}
    , timer_{new QTimer(this)}
    , api_{Utils::NetworkRequestSingleton::instance().api()}
{
}

std::optional<QNetworkReply *> Refresher::refresh()
{
    if (refreshToken_.isEmpty())
    {
        constexpr auto error{"Refresh token is empty."_L1};
        qWarning() << "Refresher::refresh: "_L1 + error;
        return std::nullopt;
    }

    QNetworkRequest request(api_->createRequest("users/refresh/"_L1));
    request.setRawHeader("Authorization", "Bearer "_ba + refreshToken_.toUtf8());

    return manager_->get(request);
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
