#ifndef REST_H
#define REST_H

#include <QtQml/qqmlregistration.h>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

namespace Cara {
    class Rest;
}

class Rest : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString base READ base WRITE base NOTIFY baseChanged )
    Q_PROPERTY(QString accessToken READ accessToken WRITE accessToken NOTIFY accessTokenChanged)
    Q_PROPERTY(QString refreshToken READ refreshToken WRITE refreshToken NOTIFY refreshTokenChanged)
    Q_PROPERTY(QVariantMap headers READ headers WRITE setHeaders NOTIFY headersChanged)
    QML_ELEMENT

public:
    enum class Verb
    {
        GET,
        POST,
        PUT,
        PATCH
    };
    Q_ENUM(Verb)

    explicit Rest(QObject *parent = nullptr);
    ~Rest();

    QString base() const;
    void base(const QString &base);

    QString accessToken() const;
    void accessToken(const QString &token);

    QString refreshToken() const;
    void refreshToken(const QString &token);

    const QVariantMap &headers() const;
    void setHeaders(const QVariantMap &headers);

signals:
    void baseChanged();
    void accessTokenChanged();
    void refreshTokenChanged();
    void headersChanged();

    void sent();
    void received(const QVariantMap &data);
    void succeeded(const QVariantMap &data);

    // HTTP code error signals
    void failed(const QVariantMap &data); // Handle any error
    void unauthorized(); // 401
    void serviceUnavailable(); // 503

private slots:
    void replyFinished();
    void errorOccurred(QNetworkReply::NetworkError error);

public slots:
    void request(Rest::Verb verb, const QString &endpoint, const QVariant &obj);

    void get(const QString &endpoint);
    void post(const QString &endpoint, const QVariantMap &obj);
    void put(const QString &endpoint, const QVariantMap &obj);
    void patch(const QString &endpoint, const QVariantMap &obj);

    void refreshAccessToken();

private:
    QString base_; // Base URL for request.
    QString code_; // Code from request; a readable one.
    QString message_; // More details about code, could be empty.
    QString accessToken_;
    QString refreshToken_;
    QVariantMap headers_;

    QNetworkAccessManager manager_;
    QNetworkReply *reply_;

    std::unique_ptr<QVariantMap> buffer_;
    std::unique_ptr<QTimer> timer_;
};

#endif // REST_H
