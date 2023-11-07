#ifndef LOCALUSERAUTHENTICATION_H
#define LOCALUSERAUTHENTICATION_H

#include "business/authentication.h"
#include <QObject>
#include <QNetworkAccessManager>

namespace Service {
class LocalUserAuthentication : public QObject, public Business::Authentication
{
    Q_OBJECT
public:
    explicit LocalUserAuthentication(QObject *parent = nullptr);

    bool isAuthenticated() const;

    QByteArray buffer() const;
    void setBuffer(const QByteArray &newBuffer);

public slots:
    void authenticate() override;

signals:
    void finished();
    void errorOccurred();

private:
    QString refreshToken_;
    bool isAuthenticated_;

    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;
    QByteArray buffer_;
};
}

#endif // LOCALUSERAUTHENTICATION_H
