#ifndef TOKENMANAGER_H
#define TOKENMANAGER_H

#include <QNetworkReply>
#include <QObject>
#include <QQmlEngine>
#include <QSettings>
#include <QTimer>


namespace Authentication::Data { class TokenStore; }

namespace Authentication::Business {
class IRefreshable;
class IToken;

class TokenManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(bool refreshing READ refreshing WRITE setRefreshing NOTIFY refreshingChanged FINAL)
    Q_PROPERTY(QString accessToken READ accessToken NOTIFY accessTokenChanged FINAL)
public:
    QString refreshToken() const;
    void setRefreshToken(const QString &newRefreshToken);

    static TokenManager *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine);
    static TokenManager *instance();

    IRefreshable *refresher() const;
    void setRefresher(IRefreshable *newRefresher);

    QString errorString() const;
    void setErrorString(const QString &newErrorString);

    bool refreshing() const;
    void setRefreshing(bool newRefreshing);

    QString accessToken() const;

public slots:
    void clear();

    void stopRefreshing();
    void resumeRefreshing();

private slots:
    void replyReady();
    void timeout();

signals:
    void accessTokenChanged();

    void refreshTokenChanged();

    void accessTokenAvailable();
    void errorOccurred();

    void refreshingChanged();

private:
    explicit TokenManager(QObject *parent = nullptr);
    void refreshAccessToken();

    QString refreshToken_;
    QTimer *timer_;
    QNetworkReply *reply_;
    QString errorString_;
    Data::TokenStore *tokenStore_;

    IRefreshable *refresher_;
    IToken *token_;

    bool refreshing_;

    static TokenManager *instance_;
};
}

#endif // TOKENMANAGER_H
