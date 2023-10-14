#ifndef TOKENMANAGER_H
#define TOKENMANAGER_H

#include <QObject>
#include <QQmlEngine>
#include <QSettings>
#include <QTimer>
#include "irefreshable.h"

namespace DataLayer {
class TokenManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QString accessToken READ accessToken WRITE setAccessToken NOTIFY accessTokenChanged FINAL)

public:
    QString accessToken() const;
    void setAccessToken(const QString &newAccessToken);

    QString refreshToken() const;
    void setRefreshToken(const QString &newRefreshToken);

    static TokenManager *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine);
    static TokenManager *instance();

public slots:
    void clear();

signals:
    void accessTokenChanged();

    void refreshTokenChanged();

    void accessTokenAvailable();

private:
    explicit TokenManager(QObject *parent = nullptr);

    QString accessToken_;
    QString refreshToken_;
    QSettings settings_;

    DataLayer::IRefreshable *refresher_;

    static TokenManager *instance_;
};
}

#endif // TOKENMANAGER_H
