#ifndef REFRESHER_H
#define REFRESHER_H

#include "business/irefreshable.h"
#include <QObject>
#include <QNetworkAccessManager>
#include <QTimer>

class QNetworkRequestFactory;

namespace Authentication::Data{
class Refresher : public QObject, public Business::IRefreshable
{
    Q_OBJECT
public:
    explicit Refresher(QObject *parent = nullptr);

    QString refreshToken() const;
    void setRefreshToken(const QString &newRefreshToken) override;

public slots:
    std::optional<QNetworkReply *> refresh() override;

signals:
    void errorOccurred(const QByteArray &data);
    void refreshTokenChanged();

private:
    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;

    QString refreshToken_;

    QTimer *timer_;
    const std::shared_ptr<QNetworkRequestFactory> api_;
};
}

#endif // REFRESHER_H
