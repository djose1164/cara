#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H

#include <QQmlEngine>

namespace Authentication::Business
{
class Authenticator : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS
public:
    enum class Error
    {
        InvalidCredentials,
        RefreshTokenExpired,
        EmptyToken,
        Unknown
    };

    using QObject::QObject;

    virtual ~Authenticator();

public slots:
    virtual void authenticate() = 0;

signals:
    void succeeded(const QByteArray &data);
    void errorOccurred(Error error, const QString &errorString);
    void tokenReady(const QString &token);
};
}

#endif // AUTHENTICATOR_H
