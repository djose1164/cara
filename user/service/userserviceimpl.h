#ifndef USERSERVICEIMPL_H
#define USERSERVICEIMPL_H

#include "userservice.h"
#include <QObject>

namespace Service {
class UserServiceImpl : public QObject, public UserService
{
    Q_OBJECT
public:
    explicit UserServiceImpl(QObject *parent = nullptr);

    void clear() override;

    const Entity::User *currentUser() const override;

signals:
    void cleared();

private:
    Entity::User *user_;
};
}

#endif // USERSERVICEIMPL_H
