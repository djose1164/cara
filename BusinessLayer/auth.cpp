#include "auth.h"
#include "constants.h"

BusinessLayer::Auth::Auth(QObject *parent)
    : QObject{parent}
{
    const QString base{Constants::instance()->base()};
    caraApi_.base(base + "/users/login");
}

void BusinessLayer::Auth::setUserEntity(EntityLayer::UserEntity *userEntity)
{
    if (userEntity_ != userEntity)
    {
        userEntity_ = userEntity;
        emit userEntityChanged();
    }
}

EntityLayer::UserEntity *BusinessLayer::Auth::userEntity() const
{
    return userEntity_;
}

void BusinessLayer::Auth::authenticate()
{
    if (!userEntity_)
        return;

    QVariantMap entity{
        {"username", userEntity_->username()},
        {"password", userEntity_->password()}
    };
    caraApi_.post("/", entity);

    QObject::connect(&caraApi_, &Rest::succeeded, this, &Auth::authenticated);
}
