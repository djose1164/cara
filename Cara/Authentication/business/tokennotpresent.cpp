#include "tokennotpresent.h"
#include <QString>

namespace Authentication
{
TokenNotPresent::TokenNotPresent(const QAnyStringView msg)
    : std::logic_error{msg.toString().toUtf8()}
{}
}
