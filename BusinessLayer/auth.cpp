#include "auth.h"

BusinessLayer::Auth::Auth(QObject *parent)
    : QObject{parent}
{
    caraApi_.base("http://127.0.0.1:5000/api/users/login");
}

void BusinessLayer::Auth::authenticate(const EntityLayer::UserEntity *userEntity)
{
    QVariantMap entity{
        {"username", userEntity->username()},
        {"password", userEntity->password()}
    };
    caraApi_.post("/", entity);

    QObject::connect(&caraApi_, &Rest::succeeded, this, &Auth::authenticated);
}
