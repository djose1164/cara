#ifndef AUTH_H
#define AUTH_H

#include "rest.h"
#include "userentity.h"
#include <QObject>
#include <QtQml>

namespace BusinessLayer {
class Auth : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(EntityLayer::UserEntity *userEntity READ userEntity WRITE setUserEntity NOTIFY userEntityChanged)

public:
    explicit Auth(QObject *parent = nullptr);

    void setUserEntity(EntityLayer::UserEntity *userEntity);
    EntityLayer::UserEntity *userEntity() const;

signals:
    void userEntityChanged();

    void authenticated(const QVariantMap &resp);
    void invalid();

public slots:
    void authenticate();

private:
    Rest caraApi_;
    EntityLayer::UserEntity *userEntity_;
};
}

#endif // AUTH_H
