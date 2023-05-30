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

public slots:
    void logOut();

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

    void loggedIn();
    void loggingOut();
    void loggedOut();

    void previousLoginFound();
    void noPreviousLoginFound();
    void restReady();

private:
    bool hasPreviousLogin_;

    Rest rest_;
    BusinessLayer::Auth auth_;

    QSettings settings_;

    static CurrentUser *instance_;
};

#endif // CURRENTUSER_H
