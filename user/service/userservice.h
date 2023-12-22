#ifndef USERSERVICE_H
#define USERSERVICE_H

namespace Entity { class UserEntity; }
namespace Service {
class UserService
{
public:
    ~UserService();

    virtual void clear() = 0;
    
    virtual const Entity::UserEntity currentUser() const = 0;
};
}

#endif // USERSERVICE_H
