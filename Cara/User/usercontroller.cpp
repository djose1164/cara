#include "usercontroller.h"
#include "contactrespositoryimpl.h"
#include "data/userrepositoryimpl.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QRestReply>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto userIdField{"user_id"_L1};
constexpr auto customerIdField{"customer_id"_L1};
constexpr auto messageField{"message"_L1};
constexpr auto newPasswordField{"new_password"_L1};
constexpr auto currentPasswordField{"current_password"_L1};
constexpr auto passwordField{"password"_L1};
}

namespace Controller
{
UserController::UserController(QObject *parent)
    : QObject{parent}
    , repository_{new Data::UserRepositoryImpl(this)}
    , contactRepository_{new Data::ContactRespositoryImpl{this}}
    , errorString_{}
{
    const auto repo{qobject_cast<Data::UserRepositoryImpl *>(repository_)};
    repo->setPath("users");

    connect(repository_, &Data::UserRepository::updated, this, &UserController::updated);
    connect(repository_, &Data::UserRepository::errorOccurred, this, [&] {
        setErrorString(repository_->errorString());
        emit errorOccurred();
    });
    connect(repository_, &Data::UserRepository::passwordUpdated,
            this, &UserController::passwordUpdated);
}

Data::UserRepository *UserController::repository() const
{
    return repository_;
}

void UserController::setRepository(Data::UserRepository *newRepository)
{
    repository_ = newRepository;
}

void UserController::update(const QVariantMap &data)
{
    const auto doc{QJsonDocument::fromVariant(data)};
    const auto userEntity{Entity::UserEntity::fromJson(doc.toJson(QJsonDocument::Compact))};
    repository_->update(userEntity);
}

void UserController::updateContact(const QVariantMap &data)
{
    QUrlQuery query;
    QVariantMap payload{data};
    if (data.contains(userIdField))
        query.addQueryItem(userIdField, payload.take(userIdField).toString());
    else if (data.contains(customerIdField))
        query.addQueryItem(customerIdField, payload.take(customerIdField).toString());
    else
    {
        setErrorString(QStringLiteral("%1 or %2 is missing.").arg(userIdField, customerIdField));
        emit errorOccurred();
        return;
    }

    contactRepository_->update(payload, query, [this](QRestReply &reply) {
        if (reply.isSuccess())
            emit contactUpdated();
        else
        {
            if (!reply.errorString().isEmpty())
            {
                setErrorString(reply.errorString());
                emit errorOccurred();
                return;
            }

            if (const auto json{reply.readJson()};
                json && json->isObject() && json->object().contains(messageField))
            {
                const auto message{(*json)[messageField].toString()};
                setErrorString(message);
                emit errorOccurred();
            }
        }
    });
}

void UserController::updatePassword(UserId userId, const QVariantMap &data)
{
    if (!validatePasswordData(data))
        return;

    repository_->updatePassword(userId, data);
}

void UserController::getUserById(int id)
{
    qDebug() << "UserController::getUserById: Request sent to read userID: " << id;
    repository_->read(id);
    connect(repository_, &Data::UserRepository::fetched, this, [&, id] {
        qDebug() << "UserController::getUserById: user with id " << id << "was fetched.";
        emit fetched(repository_->buffer());
    });
}

void UserController::readFavorites(UserId id)
{
    const auto reply{repository_->readFavorites(id)};
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        if (reply->error() != QNetworkReply::NoError)
        {
            setErrorString(reply->errorString());
            emit errorOccurred();
            return;
        }

        const auto doc{QJsonDocument::fromJson(reply->readAll())};
        if (!doc.isEmpty())
            emit fetched(doc.toJson());
    });
}

bool UserController::validatePasswordData(const QVariantMap &data)
{
    if (!data.contains(passwordField))
    {
        setErrorString("password property is missing."_L1);
        emit errorOccurred();
        return false;
    }
    else if (const auto password{data[passwordField].toMap()};
             !password.contains(currentPasswordField)
             || !password.contains(newPasswordField))
    {
        setErrorString("%1 or %2 is missing."_L1.arg(currentPasswordField, newPasswordField));
        emit errorOccurred();
        return false;
    }

    return true;
}

QString UserController::errorString() const
{
    return errorString_;
}

void UserController::setErrorString(const QString &newErrorString)
{
    if (errorString_ == newErrorString)
        return;
    errorString_ = newErrorString;
    emit errorStringChanged();
}

Entity::UserEntity UserController::create(const QVariantMap &data) const
{
    Q_UNUSED(data)
    return {};
}

Entity::UserEntity UserController::create(const QByteArray &data) const
{
    return Entity::UserEntity::fromJson(data);
}

Entity::UserEntity UserController::createEmpty() const
{
    return {};
}
}
