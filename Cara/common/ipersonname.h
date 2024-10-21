#ifndef IPERSONNAME_H
#define IPERSONNAME_H

#include <QString>

class IPersonName
{
public:
    virtual ~IPersonName() = default;

    virtual QString forename() const = 0;
    virtual QString surname() const = 0;
    virtual QString fullname() const = 0;
};

#endif // IPERSONNAME_H
