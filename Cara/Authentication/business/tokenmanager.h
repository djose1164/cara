#ifndef TOKENMANAGER_H
#define TOKENMANAGER_H

#include "istore.h"
#include <QNetworkReply>
#include <QObject>
#include <QProperty>
#include <QQmlEngine>
#include <QSettings>
#include <QTimer>

namespace Authentication::Business {
class IRefreshable;
class IToken;

class TokenManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Store::IStore *store READ store NOTIFY storeChanged FINAL)
    Q_PROPERTY(bool refreshing READ refreshing WRITE setRefreshing NOTIFY refreshingChanged FINAL)
    Q_PROPERTY(QString accessToken READ accessToken NOTIFY accessTokenChanged FINAL)
    Q_PROPERTY(QString refreshToken READ refreshToken NOTIFY refreshTokenChanged BINDABLE bindableRefreshToken FINAL)
    QML_ELEMENT
    QML_SINGLETON
public:
    QString refreshToken() const;
    void setRefreshToken(const QString &newRefreshToken);
    QBindable<QString> bindableRefreshToken() const;

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

    QString accessToken() const;
    void setAccessToken(const QString &newAccessToken);

    void setToken(IToken *newToken);

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

    QProperty<QString> refreshToken_;
    QString accessToken_;
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
