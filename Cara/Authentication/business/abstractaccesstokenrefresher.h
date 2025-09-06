#ifndef ABSTRACTACCESSTOKENREFRESHER_H
#define ABSTRACTACCESSTOKENREFRESHER_H

#include <QObject>

class QNetworkReply;
class QString;

namespace Authentication
{

class AbstractAccessTokenRefresher : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    virtual ~AbstractAccessTokenRefresher() = default;

    virtual std::optional<QNetworkReply *> refresh() = 0;
    virtual void setRefreshToken(const QString &newRefreshToken) = 0;

signals:
    void errorOccurred(const QString &errorString);
    void refreshed(const QString &newToken);
};

} //namespace Authentication

#endif // ABSTRACTACCESSTOKENREFRESHER_H
