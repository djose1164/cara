#ifndef USERSTORE_H
#define USERSTORE_H

#include "userentity.h"
#include <QQmlEngine>

namespace Business {
/**
 * @brief The UserStore class saves the user's information.
 */
class UserStore
{
    QML_INTERFACE
public:
    virtual ~UserStore();

public slots:
    virtual void save(const QByteArray &data) = 0;

    virtual std::optional<Entity::UserEntity> restore() = 0;
};
}
Q_DECLARE_INTERFACE(Business::UserStore, "UserStore")
#endif // USERSTORE_H
