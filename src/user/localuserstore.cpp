#include "localuserstore.h"
#include "salesperson.h"
#include "userentity.h"
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

void LocalUserStore::save(const QByteArray &data)
{
    const auto obj{QJsonDocument::fromJson(data).object()};
    const auto map{obj.toVariantMap()};
    const auto userJson{QJsonDocument::fromVariant(obj["user"].toVariant()).toJson()};
    const auto salespersonObj{obj["user"]["salesperson"]};
    
    setUser(Entity::UserEntity::fromJson(userJson));

    QSettings settings;
    settings.beginGroup("User");
    settings.setValue("userTypeId", int(user_.userType()));
    settings.setValue("userId", user_.userId());
    settings.setValue("username", user_.username());
    settings.setValue("accessToken", "Bearer " + map["access_token"].toString());
    settings.setValue("refreshToken", "Bearer " + map["refresh_token"].toString());
    settings.endGroup();

    settings.beginGroup("Contact");
    const auto contactData{obj["user"]["contact"].toObject()};

    settings.setValue("forename", contactData["forename"].toString());
    settings.setValue("surname", contactData["surname"].toString());
    settings.setValue("email", contactData["email"].toString());
    settings.endGroup();

    settings.beginGroup("Salesperson");
    settings.setValue("salespersonId", salespersonObj["id"].toInt());
    settings.setValue("creditLimit", salespersonObj["credit_limit"].toInt());
    settings.setValue("creditAvailable",
                      salespersonObj["credit_available"].toInt());
    settings.setValue("creditConsumed",
                      salespersonObj["credit_consumed"].toInt());
    settings.setValue("salespersonTypeId",
                      salespersonObj["salesperson_type"]["id"].toInt());
    settings.setValue("salespersonTypeName",
                      salespersonObj["salesperson_type"]["name"].toString());
    settings.endGroup();
}

void LocalUserStore::createFromStore()
{
    using UserTypes = Entity::UserEntity::UserTypes;
    QSettings settings;

    settings.beginGroup("Contact");
    user_.setForename(settings.value("forename").toString());
    user_.setSurname(settings.value("surname").toString());
    settings.endGroup();

    settings.beginGroup("User");
    bool userTypeAvailable{};
    const int userTypeId{settings.value("userTypeId").toInt(&userTypeAvailable)};
    const int guestType{int(Entity::UserEntity::UserTypes::Guest)};
    user_.setUserType(static_cast<UserTypes>(userTypeAvailable ? userTypeId : guestType));

    user_.setUsername(settings.value("username").toString());
    user_.setCustomerId(settings.value("customerId").toInt());
    user_.setUserId(settings.value("userId").toInt());
    user_.setRefreshToken(settings.value("refreshToken").toString());
    user_.setAccessToken(settings.value("accessToken").toString());
    settings.endGroup();

    using SalespersonTypes = Entity::Salesperson::SalespersonTypes;
    settings.beginGroup("Salesperson");
    Entity::Salesperson sp{};
    sp.setSalespersonId(settings.value("salespersonId").toInt());
    sp.setCreditAvailable(settings.value("creditAvailable").toInt());
    sp.setCreditLimit(settings.value("creditLimit").toInt());
    sp.setCreditConsumed(settings.value("creditConsumed").toInt());
    const int sptypeId{settings.value("salespersonTypeId").toInt()};
    sp.setSalespersonType(
        static_cast<SalespersonTypes>(
            sptypeId
    ));
    settings.endGroup();

    user_.setSalesperson(sp);
    user_.setSalespersonId(sp.salespersonId());
    emit userChanged();
}

void LocalUserStore::update()
{
    createFromStore();
}

Entity::UserEntity LocalUserStore::user() const
{
    return user_;
}

void LocalUserStore::setUser(const Entity::UserEntity &newUser)
{
    if (user_ == newUser)
        return;
    user_ = newUser;
    emit userChanged();
}

std::optional<Entity::UserEntity> LocalUserStore::restore()
{
    if (user_.username().isEmpty())
        return std::nullopt;
    return user_;
}

bool LocalUserStore::hasUser() const
{
    QSettings settings;
    return settings.contains("User/userId") && settings.contains("User/refreshToken");
}
}
