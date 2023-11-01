#ifndef USER_H
#define USER_H

#include "entity/userentity.h"
#include <QObject>
#include <QtQml>

namespace Business {
class User : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(Entity::User user READ user WRITE setUser NOTIFY userChanged FINAL)

public:
    explicit User(QObject *parent = nullptr);

    QString accessToken() const;
    void setAccessToken(const QString &token);
    QBindable<QString> bindableAccessToken();

    Entity::User user() const;
    void setUser(Entity::User user);

    bool isAdmin() const;
    QBindable<bool> bindableIsAdmin();

    bool isCustomer() const;
    QBindable<bool> bindableIsCustomer();

    static User *instance();

public slots:
    void save(const QVariantMap &user);
    void clear();

private:
    void setup();
    bool existPreviousLogin();

signals:
    void userChanged();
    void isCustomerChanged();
    void isAdminChanged();
    void accessTokenChanged();

    void refreshTokenNotAvailable();
    void UserReady();
    void completed();

    void previousLoginFound();
    void noPreviousLoginFound();

private:
    bool hasPreviousLogin_;

    Q_OBJECT_BINDABLE_PROPERTY(User, QString, accessToken_, &User::accessTokenChanged)
    Q_OBJECT_BINDABLE_PROPERTY(User, bool, isAdmin_, &User::isAdminChanged)
    Q_OBJECT_BINDABLE_PROPERTY(User, bool, isCustomer_, &User::isCustomerChanged)

    Entity::User user_;
};
}

#endif // USER_H
