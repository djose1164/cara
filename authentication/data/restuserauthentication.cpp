#include "restuserauthentication.h"
#include <QSettings>
#include <QNetworkReply>

namespace Data {
RestUserAuthentication::RestUserAuthentication(User::Entity::UserCredentail userCredentail, QObject *parent)
    : QObject{parent}
    , userCredentail_{userCredentail}
    , data_{}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{}
{

}

QByteArray RestUserAuthentication::data() const
{
    return data_;
}

void RestUserAuthentication::authenticate()
{
    qDebug() << "RestUserAuthentication::authenticate: Retreiving base & preparing request";
    QSettings settings;
    const QUrl url{settings.value("base").toString() + "/users/login"};
    QNetworkRequest request{url};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    reply_ = manager_->post(request, userCredentail_.toJson());
    qDebug() << " RestUserAuthentication::authenticate: Authentication request sent";
    connect(reply_, &QNetworkReply::finished, this, [&] {
        data_ = reply_->readAll();
        emit finished();

        reply_->deleteLater();
    });
}
}
