#include "userrepositoryimpl.h"
#include <QSettings>
#include <QNetworkReply>

namespace Data
{
UserRepositoryImpl::UserRepositoryImpl(QObject *parent)
    : UserRepository{parent}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{}
{

}

void UserRepositoryImpl::read(int id)
{
    const auto request{createRequest(QString("/%1").arg(id))};

    reply_ = manager_->get(request);

    connect(reply_, &QNetworkReply::finished, this, [&, id] {
        setBuffer(reply_->readAll());
        if (reply_->error() != QNetworkReply::NoError)
        {
            setErrorString(reply_->errorString());
            emit errorOccurred();
        }
        else
            emit fetched();

        reply_->deleteLater();
    });
}

void UserRepositoryImpl::update(const Entity::UserEntity &user)
{

}

QNetworkRequest UserRepositoryImpl::createRequest(const QString &endpoint)
{
    const QUrl url{base() + endpoint};
    QNetworkRequest request{url};

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QSettings settings;
    const QString token{settings.value("User/accessToken").toString()};
    request.setRawHeader("Authorization", token.toUtf8());

    return request;
}
}
