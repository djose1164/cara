#ifndef IREFRESHABLE_H
#define IREFRESHABLE_H

#include <optional>

class QNetworkReply;
class QString;

namespace Authentication::Business {
class IRefreshable
{
public:
    virtual ~IRefreshable();

    virtual std::optional<QNetworkReply *> refresh() = 0;
    virtual void setRefreshToken(const QString &newRefreshToken) = 0;
};
}

#endif // IREFRESHABLE_H
