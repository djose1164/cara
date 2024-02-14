#ifndef UTILS_NOTIMPLEMENTATED_H
#define UTILS_NOTIMPLEMENTATED_H

#include <stdexcept>

namespace Utils
{
class NotImplementated : public std::logic_error
{
public:
    NotImplementated()
        : std::logic_error{"Function not yet implemented"}
    {}
};
}

#endif // UTILS_NOTIMPLEMENTATED_H
