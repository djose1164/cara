#include "login.h"
#include "entity/usercredentail.h"
#include "data/restuserauthentication.h"
#include "service/localuserauthentication.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QSettings>

namespace Business {
using User::Entity::UserCredentail;

Login::Login(QObject *parent)
    : QObject{parent}
    , status_{Status::UnAuthenticated}
    , data_{}
    , authenticator_{}
    , userStore_{}
{
    QSettings settings;
    QString token{settings.value("Credentails/refreshToken").toString()};

    setHasPreviousLogin(!token.isEmpty());
}

void Login::authenticateCredentails(const QVariantMap &credentails)
{    
    const auto credentials{UserCredentail::fromMap(credentails)};
    if (!credentials.isValid())
    {
        emit errorOccurred();
        return;
    }

    // TODO: Think whether ResetUserAuthentication should be a QML_ELEMENT
    authenticator_ = new Data::RestUserAuthentication(this);
    authenticator_->authenticate();

    const auto restAuth{static_cast<Data::RestUserAuthentication *>(authenticator_)};
    connect(restAuth, &Data::RestUserAuthentication::finished, this, [=] {
        setData(restAuth->data());
        const auto obj{QJsonDocument::fromJson(data_).object()};

        if (obj.value("code").toString() == "success")
        {
            save();
            emit authenticated();
            setStatus(Status::Authenticated);
        }
        else
        {
            emit errorOccurred();
            setStatus(Status::Error);
        }

        restAuth->deleteLater();
    });
}

void Login::authenticateRefreshToken(const QString &token)
{
//    QQmlEngine engine;
//    const auto constants{engine.singletonInstance<DataLayer::Constants *>("data", "Constants")};
//    const QString base{constants->base()};
    if (token.isEmpty())
    {
        emit errorOccurred();
        return;
    }

    authenticator_ = new Service::LocalUserAuthentication(this);
    authenticator_->authenticate();

    const auto localAuth{static_cast<Service::LocalUserAuthentication *>(authenticator_)};
    connect(localAuth, &Service::LocalUserAuthentication::finished, this, [=] {
        if (localAuth->isAuthenticated())
            emit authenticated();
        localAuth->deleteLater();
    });
}

void Login::save()
{
    if (!userStore_)
        return;

    userStore_->save(data_);
}

void Login::setHasPreviousLogin(bool newHasPreviousLogin)
{
    if (hasPreviousLogin_ == newHasPreviousLogin)
        return;
    hasPreviousLogin_ = newHasPreviousLogin;
    emit hasPreviousLoginChanged();
}

bool Login::hasPreviousLogin() const
{
    return hasPreviousLogin_;
}

UserStore *Login::userStore() const
{
    return userStore_;
}

void Login::setUserStore(UserStore *newUserStore)
{
    if (userStore_ == newUserStore)
        return;
    userStore_ = newUserStore;
    emit userStoreChanged();
}

QByteArray Login::data() const
{
    return data_;
}

void Login::setData(const QByteArray &newData)
{
    if (data_ == newData)
        return;
    data_ = newData;
    emit dataChanged();
}

Login::Status Login::status() const
{
    return status_;
}

void Login::setStatus(Status newStatus)
{
    if (status_ == newStatus)
        return;
    status_ = newStatus;
    emit statusChanged();
}
}
