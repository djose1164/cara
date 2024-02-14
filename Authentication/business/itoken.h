#ifndef ITOKEN_H
#define ITOKEN_H

class QString;

namespace Authentication::Business
{
class IToken
{
public:
    virtual ~IToken() = default;

    virtual void setAccessToken(const QString &newAccessToken) = 0;
};
} // namespace User::Business

#endif // ITOKEN_H
