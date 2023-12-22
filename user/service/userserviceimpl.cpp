#include "userserviceimpl.h"
#include "userentity.h"
#include <QSettings>

namespace Service {
UserServiceImpl::UserServiceImpl(QObject *parent)
    : QObject{parent}
    , user_{}
{
    using UserTypes = Entity::UserEntity::UserTypes;
    QSettings settings;

    user_.setForename(settings.value("Contact/forename").toString());
    user_.setSurname(settings.value("Contact/surname").toString());

    settings.beginGroup("Credentials");
    user_.setUserType(static_cast<UserTypes>(settings.value("userTypeId").toInt()));
    user_.setUsername(settings.value("username").toString());
    user_.setCustomerId(settings.value("customerId").toInt());
    user_.setUserId(settings.value("userId").toInt());
    settings.endGroup();
}

void UserServiceImpl::clear()
{
    QSettings settings;
    settings.remove("User");
    settings.remove("Contact");
    settings.remove("Salesperson");
    settings.remove("Credentials");
    emit cleared();
}

const Entity::UserEntity UserServiceImpl::currentUser() const
{
    return user_;
}
}
