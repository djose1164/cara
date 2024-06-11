#ifndef RESTUSERAUTHENTICATION_H
#define RESTUSERAUTHENTICATION_H

#include "business/authenticator.h"
#include "entity/credential.h"
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequestFactory>

namespace Authentication::Data
{
class RestUserAuthentication : public Business::Authenticator
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(Entity::Credential credentials READ credentials WRITE setCredentials NOTIFY credentialsChanged FINAL REQUIRED)
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged FINAL)
public:
    explicit RestUserAuthentication(QObject *parent = nullptr);

    QByteArray data() const;

    Entity::Credential credentials() const;
    void setCredentials(const Entity::Credential &newCredentials);

    void authenticate() override;

    QString errorString() const;
    void setErrorString(const QString &newErrorString);

signals:
    void userCredentialChanged();

    void credentialsChanged();

    void errorStringChanged();

private:
    Entity::Credential credentials_;
    QString errorString_;

    QByteArray data_;

    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;
    const std::shared_ptr<QNetworkRequestFactory> api_;
};
}

#endif // RESTUSERAUTHENTICATION_H
