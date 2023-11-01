#ifndef LOCALUSERAUTHENTICATION_H
#define LOCALUSERAUTHENTICATION_H

#include "business/authentication.h"
#include <QObject>

namespace Service {
class LocalUserAuthentication : public QObject, public Business::Authentication
{
    Q_OBJECT
public:
    explicit LocalUserAuthentication(QObject *parent = nullptr);

    bool isAuthenticated() const;

public slots:
    void authenticate() override;

signals:
    void finished();

private:
    QString refreshToken_;
    bool isAuthenticated_;
};
}

#endif // LOCALUSERAUTHENTICATION_H
