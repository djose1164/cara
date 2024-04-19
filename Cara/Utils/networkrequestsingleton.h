#ifndef NETWORKREQUESTSINGLETON_H
#define NETWORKREQUESTSINGLETON_H

#include "../authentication/business/itoken.h"
#include <QNetworkAccessManager>
#include <QNetworkRequestFactory>

namespace Utils
{
class NetworkRequestSingleton : public Authentication::Business::IToken
{
public:
    static NetworkRequestSingleton &instance();

    NetworkRequestSingleton(const NetworkRequestSingleton &) = delete;
    NetworkRequestSingleton operator=(const NetworkRequestSingleton &) = delete;

    QString accessToken() const;
    void setAccessToken(const QString &newAccessToken) override;

    std::shared_ptr<QNetworkRequestFactory> api() const;

private:
    NetworkRequestSingleton();
    ~NetworkRequestSingleton() = default;

private:
    std::shared_ptr<QNetworkRequestFactory> api_;
};
}

#endif // NETWORKREQUESTSINGLETON_H
