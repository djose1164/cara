#ifndef CURRENTUSER_H
#define CURRENTUSER_H

#include "auth.h"
#include "userentity.h"

#include <QObject>
#include <QtQml>

class CurrentUser : public EntityLayer::UserEntity
{
    Q_OBJECT

    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QString accessToken READ accessToken WRITE setAccessToken NOTIFY accessTokenChanged)

public:
    static CurrentUser *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine);
    static CurrentUser *instance();

    QString accessToken() const;
    void setAccessToken(const QString &token);
    QBindable<QString> bindableAccessToken();

    Q_INVOKABLE bool isLoggedIn() const;

public slots:
    void logOut();
    void fillUserInfo(const QVariantMap &user);
    void save();

private:
    CurrentUser();

    void setup();
    void initRest(const QString &refreshToken);
    bool existPreviousLogin();

signals:
    void userEntityChanged();
    void isCustomerChanged();
    void accessTokenChanged();

    void refreshTokenNotAvailable();
    void userEntityReady();
    void completed();

    void loggingIn();
    void loggedIn();
    void loggingOut();
    void loggedOut();

    void previousLoginFound();
    void noPreviousLoginFound();
    void restReady(const QString &accessToken);

private:
    bool hasPreviousLogin_;

    Rest rest_;
    BusinessLayer::Auth auth_;

    QSettings settings_;
    Q_OBJECT_BINDABLE_PROPERTY(CurrentUser, QString, accessToken_, &CurrentUser::accessTokenChanged)

    static CurrentUser *instance_;

    Q_OBJECT_BINDABLE_PROPERTY(CurrentUser, bool, isCustomer_, &CurrentUser::isCustomerChanged)
};

#endif // CURRENTUSER_H
