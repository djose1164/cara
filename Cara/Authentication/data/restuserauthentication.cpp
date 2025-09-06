#include "restuserauthentication.h"
#include "networkrequestsingleton.h"
#include "httpstatuscodes.h"
#include <QSettings>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

using namespace Qt::StringLiterals;

namespace Authentication::Data
{
RestUserAuthentication::RestUserAuthentication(QObject *parent)
    : Business::Authenticator{parent}
    , credentials_{}
    , errorString_{}
    , data_{}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{}
    , api_{Network::NetworkRequestSingleton::instance().api()}
{
}

QByteArray RestUserAuthentication::data() const
{
    return data_;
}

void RestUserAuthentication::authenticate()
{
    qDebug() << "RestUserAuthentication::authenticate: authenticating...";
    if (!credentials_.isValid())
    {
        setErrorString("RestUserAuthentication::authenticate: credentials not valid"_L1);
        qCritical() << errorString_;
        emit errorOccurred(Authenticator::Error::InvalidCredentials, errorString_);
        return;
    }

    const QNetworkRequest request{api_->createRequest("/users/login"_L1)};

    reply_ = manager_->post(request, credentials_.toJson());

    connect(reply_, &QNetworkReply::finished, this, [&] {
        reply_->deleteLater();
        data_ = reply_->readAll();

        const auto replyError{reply_->error()};
        if (replyError == QNetworkReply::AuthenticationRequiredError or replyError == QNetworkReply::ContentNotFoundError)
        {
            emit errorOccurred(
                Authenticator::Error::InvalidCredentials,
                reply_->errorString()
            );
            return;
        }
        else if (replyError != QNetworkReply::NoError)
        {
            emit errorOccurred(
                Authenticator::Error::Unknown,
                reply_->errorString()
            );
            return;
        }

        const auto obj{QJsonDocument::fromJson(data_).object()};
        const QString codeString{obj.value("code").toString()};

        if (codeString == "success"_L1 && obj.contains("user"_L1))
        {
            emit succeeded(data_);
            Q_ASSERT(obj["access_token"_L1].toString().isEmpty() == false);
            emit tokenReady(obj["access_token"_L1].toString());
        }
        else
        {
            const int code{reply_->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()};
            const auto httpCode{static_cast<HttpStatus::Code>(code)};
            emit errorOccurred(Authenticator::Error::Unknown, HttpStatus::reasonPhrase(httpCode));
        }
        qDebug() << "RestUserAuthentication::authenticate: authenticating...Done!";
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
