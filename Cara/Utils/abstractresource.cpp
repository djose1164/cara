#include "abstractresource.h"

AbstractResource::AbstractResource(QObject *parent)
    : QObject{parent}
    , manager_{}
    , api_{}
{}

void AbstractResource::setManager(const Manager &newManager)
{
    manager_ = newManager;
}

void AbstractResource::setApi(const Api &newApi)
{
    api_ = newApi;
}

