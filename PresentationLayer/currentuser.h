#ifndef CURRENTUSER_H
#define CURRENTUSER_H

#include "auth.h"
#include "userentity.h"

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
    CurrentUser();

    bool isAdmin() const;
    bool isUser() const;
    bool isCustomer() const;
    bool isGuest() const;
    
    void setUserEntity(EntityLayer::UserEntity *userEntity);
    const EntityLayer::UserEntity *userEntity() const;
    EntityLayer::UserEntity *userEntity();

public slots:
    void setIsAdmin(bool isAdmin);
    void setIsUser(bool isUser);
    void setIsCustomer(bool isCustomer);
    void setIsGuest(bool isGuest);

private:
    void initRest(const QString &refreshToken);

signals:
    void isAdminChanged();
    void isUserChanged();
    void isCustomerChanged();
    void isGuestChanged();
    void userEntityChanged();

    void refreshTokenNotAvailable();
    void userEntityReady();

private:
    bool isAdmin_;
    bool isUser_;
    bool isCustomer_;
    bool isGuest_;
    
    EntityLayer::UserEntity *userEntity_;

    Rest rest_;
    BusinessLayer::Auth auth_;

    QSettings settings_;
};

#endif // CURRENTUSER_H
