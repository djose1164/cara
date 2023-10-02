#include "auth.h"
#include "constants.h"

BusinessLayer::Auth::Auth(QObject *parent)
    : QObject{parent}
    , caraApi_{new Rest(this)}
{
    connect(caraApi_, &Rest::unauthorized, this, &Auth::invalidCredentials);
    connect(caraApi_, &Rest::notFound, this, &Auth::notFound);
    connect(caraApi_, &Rest::finished, this, &Auth::authenticationDone);
    connect(caraApi_, &Rest::serviceUnavailable, this, &Auth::serviceUnavailable);
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

    const QString base{Constants::instance()->base()};
    caraApi_->base(base + "/users/login");
    caraApi_->post("/", entity);

    connect(caraApi_, &Rest::succeeded, this, &Auth::authenticated);
}

void BusinessLayer::Auth::authenticate(const QString &token)
{
    const QString base{Constants::instance()->base()};
    caraApi_->base(base + "/users/refresh");
    caraApi_->setHeaders({{"Authorization", token}});
    caraApi_->get("/");

    connect(caraApi_, &Rest::succeeded, this, &Auth::authenticatedWithToken);
}

void BusinessLayer::Auth::reset()
{
    caraApi_->accessToken("");
}
