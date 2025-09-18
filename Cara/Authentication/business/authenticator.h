#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H

#include <QQmlEngine>

namespace Authentication
{
class Authenticator : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS
public:
    enum class Error
    {
        InvalidCredentials,  // user/pass or token rejected
        RefreshTokenExpired, // Refresh token expired or invalid
        InvalidState,        // misuse of API (e.g. empty token)
        NetworkUnavailable,  // REST backend can’t connect
        Unknown
    };

    using QObject::QObject;

    virtual ~Authenticator() = default;

public slots:
    virtual void authenticate() = 0;

signals:
    void authenticationSucceeded(const QByteArray &data);
    void tokenGenerated(const QString &token);
    void errorOccurred(Error error, const QString &errorString);
};
}

#endif // AUTHENTICATOR_H
