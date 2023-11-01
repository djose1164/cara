#ifndef USERSERVICE_H
#define USERSERVICE_H

namespace Entity { class User; }
namespace Service {
class UserService
{
public:
    ~UserService();

    virtual void clear() = 0;

    virtual const Entity::User *currentUser() const = 0;
};
}

#endif // USERSERVICE_H
