#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

namespace Business {
class Authentication
{
public:
    virtual ~Authentication();

    virtual void authenticate() = 0;
};
}

#endif // AUTHENTICATION_H
