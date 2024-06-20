#include "userrepositoryimpl.h"
#include "Network/networkrequestsingleton.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QRestAccessManager>
#include <QRestReply>
#include <QSettings>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto resourceIdentificator{"/%1"_L1};
constexpr auto userIdField{"userId"_L1};
constexpr auto messageField{"message"_L1};
constexpr auto signupPath{"/signup"_L1};
constexpr auto patchVerb{QByteArrayView{"PATCH"}};
}

namespace Data
{
UserRepositoryImpl::UserRepositoryImpl(QObject *parent)
    : UserRepository{parent}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{}
    , path_{}
    , api_{Network::NetworkRequestSingleton::instance().api()}
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
            emit errorOccurred(HttpStatus::networkErrorToStatusCode(reply_->error()));
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
        patchVerb.toByteArray(),
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
                emit errorOccurred(HttpStatus::networkErrorToStatusCode(reply_->error()));
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
            emit errorOccurred(HttpStatus::networkErrorToStatusCode(reply_->error()));
            return;
        }

        emit deleted();
        reply_->deleteLater();
    });
}

void UserRepositoryImpl::createUser(const QVariantMap &newUser)
{
    const auto restManager{ new QRestAccessManager{manager_} };
    restManager->post(api_->createRequest(path_ + signupPath),
                     newUser,
                     this,
                     [this, restManager](QRestReply &reply){
                          restManager->deleteLater();

                         if (reply.isSuccess())
                         {
                             emit userCreated();
                             return;
                         }

                         if (const auto json{reply.readJson()};
                             json && json->isObject() && json->object().contains(messageField))
                         {
                             setErrorString((*json)[messageField].toString());
                             emit errorOccurred(HttpStatus::networkErrorToStatusCode(reply_->error()));
                         }
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
