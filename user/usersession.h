#ifndef USERSESSION_H
#define USERSESSION_H

#include "session.h"
#include "userentity.h"
#include <QObject>
#include <QQmlEngine>

class UserSession : public QObject, public Session
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    QML_IMPLEMENTS_INTERFACES(Session)

    Q_PROPERTY(Entity::UserEntity user READ user WRITE setUser NOTIFY userChanged FINAL)
    Q_PROPERTY(Session *session READ session CONSTANT FINAL)
public:
    explicit UserSession(QObject *parent = nullptr);

    Entity::UserEntity user() const;
    void setUser(const Entity::UserEntity &newUser);

    Session *session();

public slots:
    void buildUser(const QByteArray &data);
    void update() override;
    void clear() override;

signals:
    void userChanged();

private:
    Entity::UserEntity user_;
};

#endif // USERSESSION_H
