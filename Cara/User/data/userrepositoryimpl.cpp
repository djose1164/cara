#include "userrepositoryimpl.h"
#include "networkrequestsingleton.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QRestReply>
#include <QSettings>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto resourceIdentificator{"/%1"_L1};
constexpr auto userIdField{"userId"_L1};
constexpr auto messageField{"message"_L1};
const auto patchVerb{"PATCH"_ba};
}

namespace Data
{
UserRepositoryImpl::UserRepositoryImpl(QObject *parent)
    : UserRepository{parent}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{}
    , path_{}
    , api_{Authentication::NetworkRequestSingleton::instance().api()}
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

QNetworkReply *UserRepositoryImpl::readFavorites(UserId id)
{
    return manager_->get(
        api_->createRequest(path_ + QString("/%1").arg(id) + "/favorites")
    );
}

void UserRepositoryImpl::update(const Entity::UserEntity &user)
{

}

void UserRepositoryImpl::updatePassword(UserId userId, const QVariantMap &data)
{
    reply_ = manager_->sendCustomRequest(
        api_->createRequest(path_ + resourceIdentificator.arg(QString::number(userId))),
        patchVerb,
        QJsonDocument::fromVariant(data).toJson()
    );

    connect(reply_, &QNetworkReply::finished, this, [&] {
        if (!QRestReply{reply_}.isSuccess())
        {
            if (const auto doc{QJsonDocument::fromJson(reply_->readAll())};
                doc.isObject() && doc.object().contains(messageField))
            {
                auto &&errMessage{doc[messageField].toString()};
                setErrorString(
                    errMessage.isEmpty() ? reply_->errorString() : errMessage
                );
                emit errorOccurred();
            }
            return;
        }

        emit passwordUpdated();
        reply_->deleteLater();
    });
}

void UserRepositoryImpl::deleteUser(UserId userId)
{
    reply_ = manager_->deleteResource(
        api_->createRequest(path_
                            + resourceIdentificator.arg(QString::number(userId)))
        );

    connect(reply_, &QNetworkReply::finished, this, [&] {
        if (!QRestReply{reply_}.isSuccess())
        {
            setErrorString(reply_->errorString());
            emit errorOccurred();
            return;
        }

        emit deleted();
        reply_->deleteLater();
    });
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
