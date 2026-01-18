#ifndef IAPIENVIRONMENTSERVICE_H
#define IAPIENVIRONMENTSERVICE_H

#include <QString>
#include <QUrl>

namespace Core
{

class IApiEnvironmentService
{
public:
    virtual ~IApiEnvironmentService() = default;

    virtual QUrl baseUrl() const = 0;
    /**
     * @brief environmentName will return the current API_ENV, that is, what type of
     * API we're currently using.
     * @return the name of API environment currently at use.
     */
    virtual QString environmentName() const = 0;

    /**
     * @brief setEnvironment set the baseUrl and environment type
     * @param env a map containing the following keys: env, baseUrl
     */
    virtual void setEnvironment(const QVariantMap &env) = 0;
    virtual bool isProduction() const = 0;
};

} // namespace Core

#endif // IAPIENVIRONMENTSERVICE_H
