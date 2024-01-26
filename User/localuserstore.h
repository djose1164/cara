#ifndef LOCALUSERSTORE_H
#define LOCALUSERSTORE_H

#include "userentity.h"
#include "userstore.h"
#include <QObject>
#include <QQmlEngine>

namespace Business {
/**
 * @brief The LocalUserStore class can save an user entity & create, if available,
 * the saved user entity in settings.
 */
class LocalUserStore : public QObject, public UserStore
{
    Q_OBJECT
    QML_ELEMENT
    QML_IMPLEMENTS_INTERFACES(Business::UserStore)

    Q_PROPERTY(Entity::UserEntity user READ user WRITE setUser NOTIFY userChanged FINAL)
public:
    LocalUserStore(QObject *parent = nullptr);
    
    Entity::UserEntity user() const;
    void setUser(const Entity::UserEntity &newUser);

    std::optional<Entity::UserEntity> restore() override;

    bool hasUser() const;

public slots:
    void save(const QByteArray &data) override;

    void createFromStore();

    void update();

signals:
    void saved();

    void userChanged();

private:
    Entity::UserEntity user_;
};
}

#endif // LOCALUSERSTORE_H
