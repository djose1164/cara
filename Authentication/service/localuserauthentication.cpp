#include "localuserauthentication.h"
#include "localuserstore.h"
#include "networkrequestsingleton.h"
#include <QSettings>
#include <QNetworkReply>

namespace Service {
LocalUserAuthentication::LocalUserAuthentication(QObject *parent)
    : QObject{parent}
    , refreshToken_{}
    , isAuthenticated_{}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{}
{
    const QString token{Business::LocalUserStore().user().refreshToken()};
    setRefreshToken(token);
}

void LocalUserAuthentication::authenticate()
{
    if (refreshToken_.isEmpty())
    {
        qCritical() << "LocalUserAuthentication::authenticate: refreshToken is empty.";
        return;
    }
    const auto api{Utils::NetworkRequestSingleton::instance().api()};

    QNetworkRequest request{api->createRequest("/users/refresh/")};
    request.setRawHeader("Authorization", refreshToken_.toUtf8());

    reply_ = manager_->get(request);

    connect(reply_, &QNetworkReply::finished, this, [&] {
        buffer_ = reply_->readAll();
        if (reply_->error() == QNetworkReply::NoError)
            isAuthenticated_ = true;
        else
            emit errorOccurred();
        emit finished();

        reply_->deleteLater();
    });
}

QString LocalUserAuthentication::refreshToken() const
{
    return refreshToken_;
}

void LocalUserAuthentication::setRefreshToken(const QString &newRefreshToken)
{
    refreshToken_ = newRefreshToken;
}

QByteArray LocalUserAuthentication::buffer() const
{
    return buffer_;
}

void LocalUserAuthentication::setBuffer(const QByteArray &newBuffer)
{
    buffer_ = newBuffer;
}

bool LocalUserAuthentication::isAuthenticated() const
{
    return isAuthenticated_;
}
}
