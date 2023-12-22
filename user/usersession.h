#ifndef USERSESSION_H
#define USERSESSION_H

#include "session.h"
#include "userentity.h"
#include "../salesperson/salesperson.h"
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
    Q_PROPERTY(Entity::Salesperson salesperson READ salesperson WRITE setSalesperson NOTIFY salespersonChanged FINAL)
public:
    explicit UserSession(QObject *parent = nullptr);

    Entity::UserEntity user() const;
    void setUser(const Entity::UserEntity &newUser);

    Session *session();

    Entity::Salesperson salesperson() const;
    void setSalesperson(const Entity::Salesperson &newSalesperson);

public slots:
    void buildUser(const QByteArray &data);
    void update() override;
    void clear() override;

signals:
    void userChanged();

    void salespersonChanged();

private:
    Entity::UserEntity user_;
    Entity::Salesperson salesperson_;
};

#endif // USERSESSION_H
