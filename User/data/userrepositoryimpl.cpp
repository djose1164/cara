#include "userrepositoryimpl.h"
#include "networkrequestsingleton.h"
#include <QSettings>
#include <QNetworkReply>

namespace Data
{
UserRepositoryImpl::UserRepositoryImpl(QObject *parent)
    : UserRepository{parent}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{}
    , api_{Utils::NetworkRequestSingleton::instance().api()}
{

}

void UserRepositoryImpl::read(int id)
{
    const auto request{api_->createRequest(path_ + QString("/%1").arg(id))};

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

QString UserRepositoryImpl::path() const
{
    return path_;
}

void UserRepositoryImpl::setPath(const QString &newPath)
{
    path_ = newPath;
}
}
