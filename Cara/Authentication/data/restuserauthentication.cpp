#include "restuserauthentication.h"
#include "networkrequestsingleton.h"
#include <QSettings>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

using namespace Qt::StringLiterals;

namespace Authentication::Data
{
RestUserAuthentication::RestUserAuthentication(QObject *parent)
    : Business::Authentication{parent}
    , credentials_{}
    , errorString_{}
    , data_{}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{}
    , api_{NetworkRequestSingleton::instance().api()}
{

}

QByteArray RestUserAuthentication::data() const
{
    return data_;
}

void RestUserAuthentication::authenticate()
{
    if (!credentials_.isValid())
    {
        qCritical() << "RestUserAuthentication::authenticate: credentials not valid";
        emit errorOccurred();
        return;
    }

    const QNetworkRequest request{api_->createRequest("/users/login"_L1)};

    reply_ = manager_->post(request, credentials_.toJson());

    connect(reply_, &QNetworkReply::finished, this, [&] {
        data_ = reply_->readAll();

        if (reply_->error() != QNetworkReply::NoError)
        {
            emit errorOccurred();
            return;
        }

        const auto obj{QJsonDocument::fromJson(data_).object()};
        const QString codeString{obj.value("code").toString()};

        if (codeString == "success" && obj.contains("user"_L1))
        {
            emit succeeded(data_);
            emit tokenReady(obj["access_token"_L1].toString());
        }
        else if (codeString == "invalidCredentials"_L1 || codeString == "notFound"_L1)
        {
            emit errorOccurred();
        }
        else
        {
            emit errorOccurred();
        }

        reply_->deleteLater();
    });
}

QString RestUserAuthentication::errorString() const
{
    return errorString_;
}

void RestUserAuthentication::setErrorString(const QString &newErrorString)
{
    if (errorString_ == newErrorString)
        return;
    errorString_ = newErrorString;
    emit errorStringChanged();
}

Entity::Credential RestUserAuthentication::credentials() const
{
    return credentials_;
}

void RestUserAuthentication::setCredentials(const Entity::Credential &newCredentials)
{
    if (credentials_ == newCredentials)
        return;
    credentials_ = newCredentials;
    emit credentialsChanged();
}
}
