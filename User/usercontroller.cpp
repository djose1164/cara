#include "usercontroller.h"
#include "data/userrepositoryimpl.h"
#include <QJsonDocument>
#include <QJsonObject>

namespace Controller
{
UserController::UserController(QObject *parent)
    : QObject{parent}
    , repository_{new Data::UserRepositoryImpl(this)}
    , errorString_{}
{
    const auto repo{qobject_cast<Data::UserRepositoryImpl *>(repository_)};
    repo->setPath("users");

    connect(repository_, &Data::UserRepository::updated, this, &UserController::updated);
    connect(repository_, &Data::UserRepository::errorOccurred, this, [&] {
        setErrorString(repository_->errorString());
        emit errorOccurred();
    });
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

void UserController::getUserById(int id)
{
    qDebug() << "UserController::getUserById: Request sent to read userID: " << id;
    repository_->read(id);
    connect(repository_, &Data::UserRepository::fetched, this, [&, id] {
        qDebug() << "UserController::getUserById: user with id " << id << "was fetched.";
        emit fetched(repository_->buffer());
    });
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
