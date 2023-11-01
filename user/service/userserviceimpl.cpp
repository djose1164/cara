#include "userserviceimpl.h"
#include "entity/userentity.h"
#include <QSettings>

namespace Service {
UserServiceImpl::UserServiceImpl(QObject *parent)
    : QObject{parent}
    , user_{new Entity::User}
{
    using UserTypes = Entity::User::UserTypes;
    QSettings settings;

    user_->setForename(settings.value("Contact/forename").toString());
    user_->setEmail(settings.value("Contact/email").toString());
    user_->setSurname(settings.value("Contact/surname").toString());

    settings.beginGroup("Credentials");
    user_->setUserType(static_cast<UserTypes>(settings.value("userTypeId").toInt()));
    user_->setUsername(settings.value("username").toString());
    user_->setCustomerId(settings.value("customerId").toInt());
    user_->setUserId(settings.value("userId").toInt());
    user_->setWarehouseId(settings.value("warehouseId").toInt());
    settings.endGroup();
}

void UserServiceImpl::clear()
{
    QSettings settings{};
    settings.clear();
    emit cleared();
}

const Entity::User *UserServiceImpl::currentUser() const
{
    return user_;
}
}
