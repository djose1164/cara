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

public:
    explicit Auth(QObject *parent = nullptr);

signals:
    void authenticated(const QVariantMap &resp);
    void invalid();

public slots:
    void authenticate(const EntityLayer::UserEntity *userEntity);

private:
    Rest caraApi_;
};
}

#endif // AUTH_H
