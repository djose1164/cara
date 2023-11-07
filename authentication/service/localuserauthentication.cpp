#include "localuserauthentication.h"
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

}

void LocalUserAuthentication::authenticate()
{
    QSettings settings;
    const QString base{settings.value("base").toString() + "/users/refresh/"};

    QNetworkRequest request{QUrl(base)};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    const QString token{settings.value("Credentials/refreshToken").toString()};
    request.setRawHeader("Authorization", token.toUtf8());

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
