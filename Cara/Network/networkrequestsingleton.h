#ifndef NETWORKREQUESTSINGLETON_H
#define NETWORKREQUESTSINGLETON_H

#include <QNetworkAccessManager>
#include <QNetworkRequestFactory>

namespace Network
{
class NetworkRequestSingleton
{
public:
    static NetworkRequestSingleton &instance();

    NetworkRequestSingleton(const NetworkRequestSingleton &) = delete;
    NetworkRequestSingleton operator=(const NetworkRequestSingleton &) = delete;

    QString accessToken() const;
    void setAccessToken(const QString &newAccessToken);

    std::shared_ptr<QNetworkRequestFactory> api() const;

private:
    NetworkRequestSingleton();
    ~NetworkRequestSingleton() = default;

private:
    std::shared_ptr<QNetworkRequestFactory> api_;
};
}

#endif // NETWORKREQUESTSINGLETON_H
