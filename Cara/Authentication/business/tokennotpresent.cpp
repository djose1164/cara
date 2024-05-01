#include "tokennotpresent.h"
#include <QString>

namespace Authentication::Business
{
TokenNotPresent::TokenNotPresent(const QAnyStringView msg)
    : std::logic_error{msg.toString().toUtf8()}
{}
}
