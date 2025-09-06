#include "localuserstore.h"
#include "userentity.h"
#include <QBindable>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto credentialsField{"Credentials"_L1};
}

namespace User::Data
{
LocalUserStore::LocalUserStore(QObject *parent)
    : Store::IStore{parent}
    , user_{}
{
}

bool LocalUserStore::save(const QByteArray &data)
{
    qDebug() << "LocalUserStore::save: saving...";
    const auto obj{QJsonDocument::fromJson(data).object()};
    const auto salespersonObj{obj["user"]["salesperson"]};

    setUser(Entity::UserEntity::fromJson(data));

    QSettings settings;

    settings.beginGroup("User");
    settings.setValue("userTypeId", int(user_.userType()));
    settings.setValue("userId", user_.userId());
    settings.setValue("username", user_.username());
    settings.endGroup();

    settings.beginGroup("Contact");
    const auto contactData{obj["user"]["person"].toObject()};

    settings.setValue("forename", contactData["forename"].toString());
    settings.setValue("surname", contactData["surname"].toString());
    settings.setValue("email", contactData["contact"]["email"].toString());
    settings.endGroup();

    qDebug() << "LocalUserStore::save: saving... Done";
    return true;
}

void LocalUserStore::restore()
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
    settings.endGroup();

    // TODO: Should User entity contain salespersonId?
    const int spId{settings.value("salespersonId").toInt()};
    user_.setSalespersonId(spId);
    emit userChanged();
}

void LocalUserStore::clear()
{
    clearSettings();
    user_.clear();
}

void LocalUserStore::update()
{
    restore();
}

void LocalUserStore::clearSettings() const
{
    QSettings settings;
    settings.remove("User");
    settings.remove("Contact");
    settings.remove("Salesperson");
    settings.remove(credentialsField);
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

bool LocalUserStore::hasUser() const
{
    qDebug() << (user_.userId() > 0);
    qDebug() << not user_.username().isEmpty();
    return user_.userId() > 0 and not user_.username().isEmpty();
}

QString LocalUserStore::refresh() const
{
    QSettings settings;
    return settings.value(credentialsField).toString();
}

}
