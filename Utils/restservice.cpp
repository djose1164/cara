#include "networkrequestsingleton.h"
#include "restservice.h"
#include <QSettings>

namespace Utils
{
RestService::RestService(QObject *parent)
    : QObject{parent}
    , url_{}
    , resources_{}
    , manager_{std::make_shared<QRestAccessManager>()}
    , api_{NetworkRequestSingleton::instance().api()}
{
    const QString token{QSettings().value("User/accessTokenNoLeading").toString()};
    api_->setBearerToken(token.toUtf8());
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
