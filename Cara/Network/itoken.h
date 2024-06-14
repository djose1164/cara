#ifndef ITOKEN_H
#define ITOKEN_H

class QString;

namespace Network
{
class IToken
{
public:
    virtual ~IToken() = default;

    virtual void setAccessToken(const QString &newAccessToken) = 0;
};
} // namespace Network

#endif // ITOKEN_H
