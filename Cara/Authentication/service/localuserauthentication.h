#ifndef LOCALUSERAUTHENTICATION_H
#define LOCALUSERAUTHENTICATION_H

#include "business/authenticator.h"
#include <QObject>
#include <QRestAccessManager>

namespace Authentication::Service
{
class LocalUserAuthentication : public Authenticator
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString refreshToken READ refreshToken WRITE setRefreshToken NOTIFY refreshTokenChanged FINAL REQUIRED)
public:
    explicit LocalUserAuthentication(QObject *parent = nullptr);

    bool isAuthenticated() const;

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

    std::unique_ptr<QRestAccessManager> manager_;
    const QString path_;
};
} // namespace Authentication::Service

#endif // LOCALUSERAUTHENTICATION_H
