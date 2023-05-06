#include "currentuser.h"

using namespace EntityLayer;
using UserTypes = EntityLayer::UserEntity::UserTypes;

CurrentUser::CurrentUser()
    : userEntity_{new UserEntity} //TODO: Does Qt's engine free the memory later?
{
    const auto refreshToken{settings_.value("Credentials/refreshToken").toString()};
    if (!refreshToken.isEmpty())
        initRest(refreshToken);

    connect(&rest_, &Rest::accessTokenChanged, this, [this](){
        rest_.get("/");
    });

    connect(&rest_, &Rest::succeeded, this, [this](const QVariantMap &resp) {
        QVariantMap user{resp["user"].toMap()};
        userEntity_->setUsername(user["username"].toString());
        userEntity_->setUserType(static_cast<UserTypes>(user["user_type_id"].toInt()));

        QVariantMap personInfo{user["person_info"].toMap()};
        userEntity_->setForename(personInfo["forename"].toString());
        userEntity_->setSurname(personInfo["surname"].toString());

        emit userEntityReady();
    });

    connect(this, &CurrentUser::userEntityChanged, this, [this](){
        const auto userType = userEntity_->userType();
        switch (userType) {
        case UserTypes::ADMIN:
            setIsAdmin(true);
            break;
        case UserTypes::USER:
            setIsUser(true);
            break;
        case UserTypes::GUEST:
            setIsGuest(true);
            break;
        default:
            qWarning() << "Invalid user type: " << userType;
            break;
        }
    });
}

void CurrentUser::setIsAdmin(bool isAdmin)
{
    if (isAdmin_ != isAdmin)
    {
        isAdmin_ = isAdmin;
        emit isAdminChanged();
    }
}

bool CurrentUser::isAdmin() const
{
    return isAdmin_;
}

void CurrentUser::setIsUser(bool isUser)
{
    if (isUser_ != isUser)
    {
        isUser_ = isUser;
        emit isUserChanged();
    }
}

bool CurrentUser::isUser() const
{
    return isUser_;
}

void CurrentUser::setIsCustomer(bool isCustomer)
{
    if (isCustomer_ != isCustomer)
    {
        isCustomer_ = isCustomer;
        emit isCustomerChanged();
    }
}

void CurrentUser::setIsGuest(bool isGuest)
{
    if (isGuest_ != isGuest)
    {
        isGuest_ = isGuest;
        emit isGuestChanged();
    }
}

bool CurrentUser::isCustomer() const
{
    return isCustomer_;
}

bool CurrentUser::isGuest() const
{
    return isGuest_;
}

void CurrentUser::setUserEntity(EntityLayer::UserEntity *userEntity)
{
    if (userEntity_ != userEntity)
    {
        userEntity_ = userEntity;
        emit userEntityChanged();
    }
}

const EntityLayer::UserEntity *CurrentUser::userEntity() const
{
    return userEntity_;
}

EntityLayer::UserEntity *CurrentUser::userEntity()
{
    return userEntity_;
}

void CurrentUser::initRest(const QString &refreshToken)
{
    const QString restBase{"http://127.0.0.1:5000/api/users"};
    rest_.base(restBase);
    rest_.refreshToken(refreshToken);
    qDebug() << "Refreshing token...";
    rest_.refreshAccessToken();
}
