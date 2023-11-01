#ifndef LOCALUSERSTORE_H
#define LOCALUSERSTORE_H

#include "entity/userentity.h"
#include "userstore.h"
#include <QObject>
#include <QQmlEngine>

namespace Business {
class LocalUserStore : public QObject, public UserStore
{
    Q_OBJECT
    QML_ELEMENT
    QML_IMPLEMENTS_INTERFACES(Business::UserStore)

    Q_PROPERTY(Entity::User user READ user WRITE setUser NOTIFY userChanged FINAL)
public:
    LocalUserStore(QObject *parent = nullptr);

    Entity::User user() const;
    void setUser(const Entity::User &newUser);

public slots:
    void save(const QByteArray &data) const override;

    void createFromStore();

signals:
    void saved();

    void userChanged();

private:
    Entity::User user_;
};
}

#endif // LOCALUSERSTORE_H
