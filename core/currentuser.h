#ifndef CURRENTUSER_H
#define CURRENTUSER_H

#include "auth.h"

#include <QObject>
#include <QtQml>

namespace BusinessLayer {
class CurrentUser : public QObject
{
    Q_OBJECT

    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QString accessToken READ accessToken WRITE setAccessToken NOTIFY accessTokenChanged)
    Q_PROPERTY(EntityLayer::UserEntity *user MEMBER user_ NOTIFY userChanged)
    Q_PROPERTY(bool isAdmin READ isAdmin NOTIFY isAdminChanged)
    Q_PROPERTY(bool isCustomer READ isCustomer NOTIFY isCustomerChanged)

public:
    CurrentUser(QObject *parent = nullptr);
    QString accessToken() const;
    void setAccessToken(const QString &token);
    QBindable<QString> bindableAccessToken();

    EntityLayer::UserEntity *user() const;
    void setUser(EntityLayer::UserEntity *const user);

    bool isAdmin() const;
    QBindable<bool> bindableIsAdmin();

    bool isCustomer() const;
    QBindable<bool> bindableIsCustomer();

    static CurrentUser *instance();

public slots:
    void fillUserInfo(const QVariantMap &user);
    void userFromSettings();
    void save();
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
    void userEntityReady();
    void completed();

    void previousLoginFound();
    void noPreviousLoginFound();

private:
    bool hasPreviousLogin_;

    Rest rest_;
    BusinessLayer::Auth auth_;

    QSettings settings_;
    Q_OBJECT_BINDABLE_PROPERTY(CurrentUser, QString, accessToken_, &CurrentUser::accessTokenChanged)
    Q_OBJECT_BINDABLE_PROPERTY(CurrentUser, bool, isAdmin_, &CurrentUser::isAdminChanged)
    Q_OBJECT_BINDABLE_PROPERTY(CurrentUser, bool, isCustomer_, &CurrentUser::isCustomerChanged)

    static CurrentUser *instance_;

    EntityLayer::UserEntity *user_;
};
}

#endif // CURRENTUSER_H
