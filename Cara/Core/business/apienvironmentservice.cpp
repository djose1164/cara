#include "apienvironmentservice.h"
#include "core_config.h"

using namespace Qt::StringLiterals;

constexpr auto apiEnvField{"API_ENV"_L1};
constexpr auto apiUrlLocator{"API_URL_%1"_L1};
constexpr auto prodEnv{"production"_L1};
constexpr auto apiEnv{"apiEnv"_L1};
constexpr auto BASE_URL{"baseUrl"_L1};
constexpr auto SETTINGS_SECTION{"REMOTE_API"_L1};
constexpr auto settingsLocator{"REMOTE_API/%1"_L1};

namespace Core
{

ApiEnvironmentService::ApiEnvironmentService(QObject *parent)
    : QObject{parent}
    , env_{Environment::Unknown}
    , apiEnv_{ApiEnvironment::Unknown}
    , baseUrl_{}
    , settings_{}
{
    if (existsPreviousApiEnv())
    {
        restorePrevious();
    }
    else
    {
        restoreCompiledEnv();
    }

    connect(this,
            &ApiEnvironmentService::baseUrlChanged,
            this,
            &ApiEnvironmentService::uploadsPathChanged);
}

ApiEnvironmentService::~ApiEnvironmentService()
{
#ifdef QT_DEBUG
    settings_.remove(SETTINGS_SECTION);
#endif
}

QString ApiEnvironmentService::environmentName() const
{
    return settings_.value(apiEnvField).toString();
}

QUrl ApiEnvironmentService::baseUrl() const
{
    return baseUrl_;
}

bool ApiEnvironmentService::isProduction() const
{
    return environmentName() == prodEnv;
}

void ApiEnvironmentService::setEnvironment(const QVariantMap &map)
{
    if (map.contains(apiEnv) && map.contains(BASE_URL))
    {
        setBaseUrl(map[BASE_URL].toUrl());
        setEnv(static_cast<ApiEnvironment>(map[apiEnv].toInt()));
    }

    const QString envName{environmentName()};
    if (envName == "local"_L1)
        env_ = Environment::Local;
    else if (envName == "local_network"_L1)
        env_ = Environment::LocalNetowrk;
}

QUrl ApiEnvironmentService::uploadsPath() const
{
    return uploadsPath_;
}

void ApiEnvironmentService::setBaseUrl(const QUrl &newBaseUrl)
{
    if (newBaseUrl == baseUrl_)
        return;
    settings_.setValue(SETTINGS_SECTION + "/baseUrl"_L1, newBaseUrl);
    baseUrl_ = newBaseUrl;
    setUploadsPath(uploadsUrl());
    emit baseUrlChanged();
}

void ApiEnvironmentService::setEnv(ApiEnvironment newApiEnv)
{
    if (apiEnv_ == newApiEnv)
        return;
    settings_.setValue(settingsLocator.arg("apiEnvironment"_L1), int(newApiEnv));
    apiEnv_ = newApiEnv;
    emit apiEnvChanged();
}

bool ApiEnvironmentService::existsPreviousApiEnv() const
{
    return !settings_.value(settingsLocator.arg("baseUrl"_L1)).toString().isEmpty();
}

ApiEnv ApiEnvironmentService::getFromSettings() const
{
    ApiEnv env;
    env.baseUrl_ = settings_.value(settingsLocator.arg("baseUrl"_L1)).toUrl();
    int apiEnvInt{settings_.value(settingsLocator.arg("apiEnvironment"_L1)).toInt()};
    env.apiEnv_ = static_cast<ApiEnvironment>(apiEnvInt);
    return env;
}

void ApiEnvironmentService::restorePrevious()
{
    auto latestEnv{getFromSettings()};
    setBaseUrl(latestEnv.baseUrl_);
    setEnv(latestEnv.apiEnv_);
}

void ApiEnvironmentService::restoreCompiledEnv()
{
    auto latestEnv{getCompiledApiEnv()};
    setBaseUrl(latestEnv.baseUrl_);
    setEnv(latestEnv.apiEnv_);
}

QUrl ApiEnvironmentService::uploadsUrl() const
{
    qDebug() << baseUrl_;
    Q_ASSERT(baseUrl_.isValid());

    QUrl url = baseUrl_;
    url.setPath(url.path() + "/uploads/"_L1);
    return url;
}

void ApiEnvironmentService::setUploadsPath(const QUrl &newUploadsPath)
{
    if (uploadsPath_ == newUploadsPath)
        return;
    uploadsPath_ = newUploadsPath;
    emit uploadsPathChanged();
}


ApiEnv ApiEnvironmentService::getCompiledApiEnv() const
{
    ApiEnv env;
    env.baseUrl_ = QStringLiteral(API_BASE_URL);
    env.apiEnv_ = environmentToApiEnvironment(QStringLiteral(API_ENVIRONMENT));
    return env;
}

} // namespace Core
