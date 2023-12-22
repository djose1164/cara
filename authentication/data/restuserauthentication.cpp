#include "restuserauthentication.h"
#include <QSettings>
#include <QNetworkReply>

namespace Data {
RestUserAuthentication::RestUserAuthentication(User::Entity::UserCredential userCredential, QObject *parent)
    : QObject{parent}
    , userCredential_{userCredential}
    , base_{QSettings().value("base").toString() + "/users/login"}
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
    const QUrl url{base_};
    QNetworkRequest request{url};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    reply_ = manager_->post(request, userCredential_.toJson());

    connect(reply_, &QNetworkReply::finished, this, [&] {
        data_ = reply_->readAll();
        emit finished();
        if (reply_->error() == QNetworkReply::NoError)
            emit succeeded();
        else if (reply_->error() == QNetworkReply::AuthenticationRequiredError)
            emit failed();

        reply_->deleteLater();
    });
}

QString RestUserAuthentication::base() const
{
    return base_;
}
}
