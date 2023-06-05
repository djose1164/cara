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

public:
    static CurrentUser *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine);
    static CurrentUser *instance();

    QString accessToken() const;
    void setAccessToken(const QString &token);
    QBindable<QString> bindableAccessToken();

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
    QProperty<QString> accessToken_;

    static CurrentUser *instance_;
};

#endif // CURRENTUSER_H
