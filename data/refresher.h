#ifndef REFRESHER_H
#define REFRESHER_H

#include "irefreshable.h"
#include <QObject>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QSettings>

namespace DataLayer {
class Refresher : public QObject, public IRefreshable
{
    Q_OBJECT

public:
    explicit Refresher(QObject *parent = nullptr);

    QString refreshToken() const;
    void setRefreshToken(const QString &newRefreshToken);

public slots:
    void refresh() override;

signals:
    void accessTokenRefreshed(const QByteArray &data);
    void errorOccurred(const QByteArray &data);

    void refreshTokenChanged();

private:
    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;

    QString refreshToken_;

    QTimer *timer_;
    QSettings settings_;
};
}

#endif // REFRESHER_H
