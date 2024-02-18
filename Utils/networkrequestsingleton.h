#ifndef NETWORKREQUESTSINGLETON_H
#define NETWORKREQUESTSINGLETON_H

#include <QNetworkAccessManager>
#include <QNetworkRequestFactory>
#include "../authentication/business/itoken.h"

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
    QNetworkAccessManager *manager() const;

private:
    NetworkRequestSingleton();
    ~NetworkRequestSingleton() = default;

private:
    std::shared_ptr<QNetworkRequestFactory> api_;
    std::shared_ptr<QNetworkAccessManager> manager_;
};
}

#endif // NETWORKREQUESTSINGLETON_H
