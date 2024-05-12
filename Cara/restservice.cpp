#include "restservice.h"
#include "networkrequestsingleton.h"
#include "qnamsingleton.h"
#include <QRestAccessManager>
#include <QSettings>

using namespace Qt::StringLiterals;

namespace Cara
{
RestService::RestService(QObject *parent)
    : QObject{parent}
    , url_{}
    , resources_{}
    , accessManager_{QNAMSingleton::instance().qnam()}
    , manager_{std::make_shared<QRestAccessManager>(accessManager_)}
    , api_{Authentication::NetworkRequestSingleton::instance().api()}
{
}

QUrl RestService::url() const
{
    return url_;
}

void RestService::setUrl(const QUrl &newUrl)
{
    if (url_ == newUrl)
        return;
    url_ = newUrl;
    emit urlChanged();
}

QRestAccessManager *RestService::manager() const
{
    return manager_.get();
}

QNetworkRequestFactory *RestService::api() const
{
    return api_.get();
}

QQmlListProperty<AbstractResource> RestService::resources()
{
    return {this, &resources_};
}

void RestService::classBegin()
{
}

void RestService::componentComplete()
{
    for (const auto resource : std::as_const(resources_))
        setupResource(*resource);
}

void RestService::append(AbstractResource *resource)
{
    setupResource(*resource);
    resources_.append(resource);
}

void RestService::setupResource(AbstractResource &r)
{
    r.setManager(manager_);
    r.setApi(api_);
}
}
