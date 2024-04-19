#ifndef TOKENNOTPRESENT_H
#define TOKENNOTPRESENT_H

#include <stdexcept>
#include <QAnyStringView>

namespace Authentication::Business
{
class TokenNotPresent : public std::logic_error
{
public:
    TokenNotPresent(const QAnyStringView msg);
};
}

#endif // TOKENNOTPRESENT_H
