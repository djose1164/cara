#ifndef ACCESSTOKENREFRESHER_H
#define ACCESSTOKENREFRESHER_H

#include "business/abstractaccesstokenrefresher.h"
#include <QObject>
#include <QProperty>
#include <QRestAccessManager>
#include <QTimer>

class QNetworkRequestFactory;

namespace Authentication::Data
{
class AccessTokenRefresher : public AbstractAccessTokenRefresher
{
    Q_OBJECT
public:
    explicit AccessTokenRefresher(QObject *parent = nullptr);

    QString refreshToken() const;
    void setRefreshToken(const QString &newRefreshToken) override;

    QString accessToken() const;
    void setAccessToken(const QString &newAccessToken);
    QBindable<QString> bindableAccessToken();

public slots:
    std::optional<QNetworkReply *> refresh() override;

signals:
    void refreshTokenChanged();
    void tokenRefreshed(const QString &token);

private:
    std::unique_ptr<QRestAccessManager> manager_;
    QNetworkReply *reply_;

    QString refreshToken_;
    const std::shared_ptr<QNetworkRequestFactory> api_;
    QProperty<QString> accessToken_;
};
}

#endif // ACCESSTOKENREFRESHER_H
