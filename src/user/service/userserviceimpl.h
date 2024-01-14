#ifndef USERSERVICEIMPL_H
#define USERSERVICEIMPL_H

#include "user/userentity.h"
#include "userservice.h"
#include <QObject>

namespace Service {
class UserServiceImpl : public QObject, public UserService
{
    Q_OBJECT
public:
    explicit UserServiceImpl(QObject *parent = nullptr);

    void clear() override;
    
    const Entity::UserEntity currentUser() const override;

signals:
    void cleared();

private:
    Entity::UserEntity user_;
};
}

#endif // USERSERVICEIMPL_H
