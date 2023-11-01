#include "restuserauthentication.h"
#include <QSettings>
#include <QNetworkReply>

namespace Data {
RestUserAuthentication::RestUserAuthentication(QObject *parent)
    : QObject{parent}
    , data_{}
{

}

QByteArray RestUserAuthentication::data() const
{
    return data_;
}

void RestUserAuthentication::authenticate()
{
    QSettings settings;

    const QUrl url{settings.value("base").toUrl()};
    QNetworkRequest request{url};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    reply_ = manager_->post(request, "");

    connect(reply_, &QNetworkReply::finished, this, [&] {

    });
}
}
