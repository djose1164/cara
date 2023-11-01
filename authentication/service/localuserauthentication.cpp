#include "localuserauthentication.h"

namespace Service {
LocalUserAuthentication::LocalUserAuthentication(QObject *parent)
    : QObject{parent}
    , refreshToken_{}
    , isAuthenticated_{}
{

}

void LocalUserAuthentication::authenticate()
{

}

bool LocalUserAuthentication::isAuthenticated() const
{
    return isAuthenticated_;
}
}
