#ifndef TOKENMANAGER_H
#define TOKENMANAGER_H

#include <QNetworkReply>
#include <QObject>
#include <QProperty>
#include <QQmlEngine>
#include <QSettings>
#include <QTimer>

namespace Authentication { class AbstractAccessTokenRefresher; }
namespace Authentication::Business
{

class TokenManager : public QObject
{
    Q_OBJECT

public:
    explicit TokenManager(QObject *parent = nullptr);
    ~TokenManager();

    QString refreshToken() const;
    void setRefreshToken(const QString &newRefreshToken);
    QBindable<QString> bindableRefreshToken() const;

    AbstractAccessTokenRefresher *refresher() const;
    void setRefresher(AbstractAccessTokenRefresher *newRefresher);

    QString errorString() const;
    void setErrorString(const QString &newErrorString);

    bool refreshing() const;
    void setRefreshing(bool newRefreshing);

    QString accessToken() const;
    void setAccessToken(const QString &newAccessToken);

public slots:
    void clear();

    void stopRefreshing();
    void resumeRefreshing();

    void start();
    void stop();

private slots:
    void timeout();

signals:
    void accessTokenChanged();

    void refreshTokenChanged();

    void accessTokenAvailable();
    void errorOccurred(const QString &errorString);

    void refreshingChanged();

    void storeChanged();

private:
    QProperty<QString> refreshToken_;
    QString accessToken_;
    QPointer<QTimer> timer_;
    QNetworkReply *reply_;
    QString errorString_;

    AbstractAccessTokenRefresher *refresher_;

    bool refreshing_;
};
}

#endif // TOKENMANAGER_H
