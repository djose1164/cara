#include "restuserauthentication.h"
#include "httpstatuscodes.h"
#include "networkrequestsingleton.h"
#include "qnamsingleton.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QRestReply>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto tokenField{"access_token"_L1};
constexpr auto loginPath{"/users/login"_L1};
}

namespace Authentication::Data
{
RestUserAuthentication::RestUserAuthentication(QObject *parent)
    : Authenticator{parent}
    , credentials_{}
    , errorString_{}
    , manager_{QNAMSingleton::instance().qram()}
    , api_{Network::NetworkRequestSingleton::instance().api()}
    , path_{loginPath}
{
}

void RestUserAuthentication::authenticate()
{
    qDebug() << "RestUserAuthentication::authenticate: authenticating...";
    if (!credentials_.isValid())
    {
        setErrorString("RestUserAuthentication::authenticate: credentials not valid"_L1);
        qCritical() << errorString_;
        emit errorOccurred(Authenticator::Error::InvalidState, errorString_);
        return;
    }

    const QNetworkRequest request{api_->createRequest(path_)};

    manager_->post(request, credentials_.toJson(), this, [&](QRestReply &reply) {

        const auto replyError{reply.error()};
        if (replyError == QNetworkReply::AuthenticationRequiredError or replyError == QNetworkReply::ContentNotFoundError)
        {
            emit errorOccurred(
                Authenticator::Error::InvalidCredentials,
                reply.errorString()
            );
            return;
        }
        else if (replyError != QNetworkReply::NoError)
        {
            emit errorOccurred(
                Authenticator::Error::Unknown,
                reply.errorString()
            );
            return;
        }

        const auto json = reply.readJson();
        QJsonObject obj;
        if (json->isObject())
            obj = json->object();
        else
            return;

        const QString codeString{obj.value("code").toString()};

        if (codeString == "success"_L1 && obj.contains("user"_L1))
        {
            emit authenticationSucceeded(json->toJson());
            Q_ASSERT(obj[tokenField].toString().isEmpty() == false);
            emit tokenGenerated(obj[tokenField].toString());
        }
        else
        {
            const auto httpCode{static_cast<HttpStatus::Code>(reply.httpStatus())};
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
