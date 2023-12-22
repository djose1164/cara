#ifndef RESTUSERAUTHENTICATION_H
#define RESTUSERAUTHENTICATION_H

#include "business/authentication.h"
#include "entity/usercredential.h"
#include <QObject>
#include <QNetworkAccessManager>

namespace Data {
class RestUserAuthentication : public QObject, public Business::Authentication
{
    Q_OBJECT
public:
    explicit RestUserAuthentication(User::Entity::UserCredential userCredential, QObject *parent = nullptr);

    QByteArray data() const;

    QString base() const;

public slots:
    void authenticate() override;

signals:
    void finished();
    void succeeded();
    void failed();

private:
    const User::Entity::UserCredential userCredential_;
    const QString base_;

    QByteArray data_;

    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;
};
}

#endif // RESTUSERAUTHENTICATION_H
