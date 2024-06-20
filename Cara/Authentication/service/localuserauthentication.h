#ifndef LOCALUSERAUTHENTICATION_H
#define LOCALUSERAUTHENTICATION_H

#include "business/authenticator.h"
#include <QObject>
#include <QNetworkAccessManager>

namespace Authentication::Service {
class LocalUserAuthentication : public Business::Authenticator
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString refreshToken READ refreshToken WRITE setRefreshToken NOTIFY refreshTokenChanged FINAL REQUIRED)
public:
    explicit LocalUserAuthentication(QObject *parent = nullptr);

    bool isAuthenticated() const;

    QByteArray buffer() const;
    void setBuffer(const QByteArray &newBuffer);

    QString refreshToken() const;
    void setRefreshToken(const QString &newRefreshToken);

public slots:
    void authenticate() override;

signals:
    void refreshTokenChanged();
    void tokenExpired();

private:
    QString refreshToken_;
    bool isAuthenticated_;

    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;
    QByteArray buffer_;
};
}

#endif // LOCALUSERAUTHENTICATION_H
