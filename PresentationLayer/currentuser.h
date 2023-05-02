#ifndef CURRENTUSER_H
#define CURRENTUSER_H

#include <../EntityLayer/userentity.h>

#include <QObject>
#include <QtQml>

class CurrentUser : public QObject
{
    Q_OBJECT

    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(bool isAdmin READ isAdmin WRITE setIsAdmin NOTIFY isAdminChanged)
    Q_PROPERTY(bool isUser READ isUser WRITE setIsUser NOTIFY isUserChanged)
    Q_PROPERTY(bool isCustomer READ isCustomer WRITE setIsCustomer NOTIFY isCustomerChanged)
    Q_PROPERTY(EntityLayer::UserEntity *userEntity READ userEntity WRITE setUserEntity NOTIFY userEntityChanged)

public:
    void setIsAdmin(bool isAdmin);
    bool isAdmin() const;

    void setIsUser(bool isUser);
    bool isUser() const;

    void setIsCustomer(bool isCustomer);
    bool isCustomer() const;
    
    void setUserEntity(EntityLayer::UserEntity *userEntity);
    const EntityLayer::UserEntity *userEntity() const;
    EntityLayer::UserEntity *userEntity();

signals:
    void isAdminChanged();
    void isUserChanged();
    void isCustomerChanged();
    void userEntityChanged();

private:
    bool isAdmin_;
    bool isUser_;
    bool isCustomer_;
    
    EntityLayer::UserEntity *userEntity_;
};

#endif // CURRENTUSER_H
