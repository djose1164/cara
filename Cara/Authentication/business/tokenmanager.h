#ifndef TOKENMANAGER_H
#define TOKENMANAGER_H

#include <QNetworkReply>
#include <QObject>
#include <QQmlEngine>
#include <QSettings>
#include <QTimer>
#include <istore.h>

namespace Authentication::Business {
class IRefreshable;
class IToken;

class TokenManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_PROPERTY(Store::IStore *store READ store WRITE setStore NOTIFY storeChanged FINAL)
    Q_PROPERTY(bool refreshing READ refreshing WRITE setRefreshing NOTIFY refreshingChanged FINAL)
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

    Store::IStore *store() const;
    void setStore(Store::IStore *newStore);

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

    void storeChanged();

private:
    explicit TokenManager(QObject *parent = nullptr);
    void refreshAccessToken();

    QString refreshToken_;
    QTimer *timer_;
    QNetworkReply *reply_;
    QString errorString_;
    Store::IStore *store_;

    IRefreshable *refresher_;
    IToken *token_;

    bool refreshing_;

    static TokenManager *instance_;
};
}

#endif // TOKENMANAGER_H
