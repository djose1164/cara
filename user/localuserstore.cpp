#include "localuserstore.h"
#include "entity/userentity.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

namespace Business {
LocalUserStore::LocalUserStore(QObject *parent)
    : QObject{parent}
    , user_{}
{
    createFromStore();
}

void LocalUserStore::save(const QByteArray &data) const
{
    const auto map{QJsonDocument::fromJson(data).object().toVariantHash()};
    const auto user{Entity::User::fromMap(map["user"].toMap())};

    QSettings settings;
    settings.beginGroup("Contact");
    settings.setValue("forename", user_.forename());
    settings.setValue("surname", user_.surname());
    settings.setValue("email", user_.email());
    settings.endGroup();

    settings.beginGroup("Credentials")    ;
    settings.setValue("username", user_.username());
    settings.setValue("userTypeId", int(user_.userType()));
    settings.setValue("customerId", user_.customerId());
    settings.setValue("userId", user_.userId());
    settings.setValue("warehouseId", user_.warehouseId());
    settings.setValue("accessToken", map["access_token"].toString());
    settings.setValue("refreshToken", map["refresh_token"].toString());
    settings.endGroup();
}

void LocalUserStore::createFromStore()
{
    using UserTypes = Entity::User::UserTypes;
    QSettings settings;
    settings.beginGroup("Contact");
    user_.setForename(settings.value("forename").toString());
    user_.setEmail(settings.value("email").toString());
    user_.setSurname(settings.value("surname").toString());
    settings.endGroup();

    settings.beginGroup("Credentials");
    user_.setUserType(static_cast<UserTypes>(settings.value("userTypeId").toInt()));
    user_.setUsername(settings.value("username").toString());
    user_.setCustomerId(settings.value("customerId").toInt());
    user_.setUserId(settings.value("userId").toInt());
    user_.setWarehouseId(settings.value("warehouseId").toInt());
    // setAccessToken(settings.value("accessToken").toString());
    settings.endGroup();
}

Entity::User LocalUserStore::user() const
{
    return user_;
}

void LocalUserStore::setUser(const Entity::User &newUser)
{
    if (user_ == newUser)
        return;
    user_ = newUser;
    emit userChanged();
}
}
