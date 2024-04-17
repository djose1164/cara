#ifndef LOCALUSERSTORE_H
#define LOCALUSERSTORE_H

#include "istore.h"
#include "userentity.h"
#include "entity/credential.h"
#include <QObject>
#include <QQmlEngine>

namespace User::Data
{
/**
 * @brief The LocalUserStore class can save an user entity & create, if available,
 * the saved user entity in settings.
 */
class LocalUserStore : public Store::IStore
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(Entity::UserEntity user READ user WRITE setUser NOTIFY userChanged FINAL)
public:
    LocalUserStore(QObject *parent = nullptr);
    
    Entity::UserEntity user() const;
    void setUser(const Entity::UserEntity &newUser);

    void clear() override;

    bool hasUser() const;

    void setPreviousLogin(bool newPreviousLogin);

    Authentication::Entity::Credential credentials() const;
    void setCredentials(const Authentication::Entity::Credential &newCredentials);

    QString refresh() const override;

public slots:
    void restore() override;
    bool save(const QByteArray &data) override;

    void update();

signals:
    void userChanged();

    void credentialsChanged();

private:
    void clearSettings() const;

private:
    Entity::UserEntity user_;
};
}

#endif // LOCALUSERSTORE_H
