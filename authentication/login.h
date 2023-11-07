#ifndef BUSINESS_LOGIN_H
#define BUSINESS_LOGIN_H

#include "userstore.h"
#include "business/authentication.h"
#include <QObject>
#include <QQmlEngine>
#include <QByteArray>

namespace Business {
class Login : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(Status status READ status WRITE setStatus NOTIFY statusChanged FINAL)
    Q_PROPERTY(QByteArray data READ data WRITE setData NOTIFY dataChanged FINAL)
    Q_PROPERTY(UserStore *userStore READ userStore WRITE setUserStore NOTIFY userStoreChanged FINAL)
    Q_PROPERTY(bool hasPreviousLogin READ hasPreviousLogin NOTIFY hasPreviousLoginChanged FINAL)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged FINAL)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged FINAL)
public:
    enum class Status {
        Authenticated,
        UnAuthenticated,
        InvalidCredentials,
        Error
    };
    Q_ENUM(Status)

    explicit Login(QObject *parent = nullptr);

    Status status() const;
    void setStatus(Status newStatus);

    QByteArray data() const;
    void setData(const QByteArray &newData);

    UserStore *userStore() const;
    void setUserStore(UserStore *newUserStore);

    bool hasPreviousLogin() const;

    QString username() const;
    void setUsername(const QString &newUsername);

    QString password() const;
    void setPassword(const QString &newPassword);

signals:
    void authenticated();
    void errorOccurred();

    void statusChanged();

    void dataChanged();

    void userStoreChanged();

    void hasPreviousLoginChanged();

    void usernameChanged();

    void passwordChanged();

public slots:
    void authenticateCredentials();
    void authenticateRefreshToken();

private:
    void save();
    void setHasPreviousLogin(bool newHasPreviousLogin);

private:
    Status status_;
    QByteArray data_;
    Authentication *authenticator_;
    UserStore *userStore_;

    bool hasPreviousLogin_;
    QString refreshToken_;

    QString username_;
    QString password_;
};
}

#endif // BUSINESS_LOGIN_H
