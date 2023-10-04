#ifndef AUTH_H
#define AUTH_H

#include "../data/rest.h"
#include "userentity.h"
#include <QObject>

namespace BusinessLayer {
class Auth : public QObject
{
    Q_OBJECT

public:
    explicit Auth(QObject *parent = nullptr);

    void setUserEntity(EntityLayer::UserEntity *userEntity);
    EntityLayer::UserEntity *userEntity() const;

signals:
    void userEntityChanged();

    void authenticated(const QVariantMap &resp);
    void authenticatedWithToken();
    void notFound();
    void invalidCredentials();
    void authenticationDone();
    void serviceUnavailable();

public slots:
    void authenticate();
    void authenticate(const QString &token);

    void reset();

private:
    Rest *caraApi_;
    EntityLayer::UserEntity *userEntity_;
};
}

#endif // AUTH_H
