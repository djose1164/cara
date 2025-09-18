#ifndef BUSINESS_LOGIN_H
#define BUSINESS_LOGIN_H

#include "business/authenticator.h"
#include "session.h"
#include <QByteArray>
#include <QObject>
#include <QProperty>
#include <QQmlEngine>

namespace Network { class IToken; };

namespace Authentication
{

class Login : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Status status READ status WRITE setStatus NOTIFY statusChanged FINAL)
    Q_PROPERTY(Authenticator *authenticator READ authenticator WRITE setAuthenticator NOTIFY authenticatorChanged FINAL REQUIRED)
    Q_PROPERTY(Authenticator::Error error BINDABLE bindableError)
    Q_PROPERTY(QString errorString READ errorString BINDABLE bindableErrorString FINAL)
    QML_ELEMENT
public:
    using Error = Authenticator::Error;

    enum class Status
    {
        Authenticated,
        UnAuthenticated,
        Busy,
    };
    Q_ENUM(Status)

    explicit Login(QObject *parent = nullptr);

    Status status() const;
    void setStatus(Status newStatus);

    Authenticator *authenticator() const;
    void setAuthenticator(Authenticator *newAuthenticator);

    QBindable<Authenticator::Error> bindableError() const;

    QString errorString() const;
    QBindable<QString> bindableErrorString() const;

signals:
    void authenticated(const Authentication::Session &session);
    void errorOccurred(Authenticator::Error error, const QString &errorString);

    void statusChanged();

    void authenticatorChanged();

public slots:
    void authenticate();

private slots:
    void userAuthenticated(const QByteArray &data);
    void authenticatorConnection();
    void authenticationFailed(Authenticator::Error error, const QString &errorString);

private:
    Status status_;
    QPointer<Authenticator> authenticator_;
    QProperty<Authenticator::Error> error_;
    QProperty<QString> errorString_;
};
}

#endif // BUSINESS_LOGIN_H
