#ifndef APIENV_H
#define APIENV_H

#include <QString>
#include <QUrl>

namespace Core
{

enum class ApiEnvironment
{
    Development,
    Staging,
    Production,
    Unknown
};

enum class Environment
{
    Local,
    LocalNetowrk,
    Testing,
    Production,
    Unknown
};

struct ApiEnv
{
    QUrl baseUrl_;
    ApiEnvironment apiEnv_;
    Environment env_;
};

static ApiEnvironment environmentToApiEnvironment(QAnyStringView envString)
{
    if (envString == QStringLiteral("production"))
        return ApiEnvironment::Production;
    else if (envString == QStringLiteral("development") || envString == QStringLiteral("testing"))
        return ApiEnvironment::Development;
    else if (envString == QStringLiteral("staging"))
        return ApiEnvironment::Staging;
    else
        return ApiEnvironment::Unknown;
}

}

#endif // APIENV_H
