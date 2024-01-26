#include "abstractresource.h"

AbstractResource::AbstractResource(QObject *parent)
    : QObject{parent}
    , manager_{}
    , api_{}
{}

void AbstractResource::setManager(const std::shared_ptr<QRestAccessManager> &newManager)
{
    manager_ = newManager;
}

void AbstractResource::setApi(const std::shared_ptr<QNetworkRequestFactory> &newApi)
{
    api_ = newApi;
}

