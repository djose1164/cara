#ifndef RESTUSERAUTHENTICATION_H
#define RESTUSERAUTHENTICATION_H

#include "business/authentication.h"
#include "entity/usercredentail.h"
#include <QObject>
#include <QNetworkAccessManager>

namespace Data {
class RestUserAuthentication : public QObject, public Business::Authentication
{
    Q_OBJECT
public:
    explicit RestUserAuthentication(QObject *parent = nullptr);

    QByteArray data() const;

public slots:
    void authenticate() override;

signals:
    void finished();

private:
    User::Entity::UserCredentail userCredentail_;
    QByteArray data_;

    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;
};
}

#endif // RESTUSERAUTHENTICATION_H
