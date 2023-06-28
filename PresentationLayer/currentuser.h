#ifndef CURRENTUSER_H
#define CURRENTUSER_H

#include "auth.h"

#include <QObject>
#include <QtQml>

class CurrentUser : public QObject
{
    Q_OBJECT

    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QString accessToken READ accessToken WRITE setAccessToken NOTIFY accessTokenChanged)
    Q_PROPERTY(EntityLayer::UserEntity *user MEMBER user_ NOTIFY userChanged)

public:
    static CurrentUser *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine);
    static CurrentUser *instance();

    QString accessToken() const;
    void setAccessToken(const QString &token);
    QBindable<QString> bindableAccessToken();

    EntityLayer::UserEntity *user() const;
    void setUser(EntityLayer::UserEntity *const user);

public slots:
    void fillUserInfo(const QVariantMap &user);
    void userFromSettings();
    void save();
    void clear();

private:
    CurrentUser();

    void setup();
    void initRest(const QString &refreshToken);
    bool existPreviousLogin();

signals:
    void userChanged();
    void isCustomerChanged();
    void accessTokenChanged();

    void refreshTokenNotAvailable();
    void userEntityReady();
    void completed();

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

    EntityLayer::UserEntity *user_;
};

#endif // CURRENTUSER_H
