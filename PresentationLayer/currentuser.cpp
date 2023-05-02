#include "currentuser.h"

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

bool CurrentUser::isCustomer() const
{
    return isCustomer_;
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
