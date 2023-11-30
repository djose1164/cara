#include "login.h"
#include "../data/tokenmanager.h"
#include "data/restuserauthentication.h"
#include "entity/usercredentail.h"
#include "localuserstore.h"
#include "service/localuserauthentication.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

namespace Business {
using User::Entity::UserCredentail;

Login::Login(QObject *parent)
    : QObject{parent}
    , status_{Status::UnAuthenticated}
    , mode_{AuthenticationMode::Undefined}
    , data_{}
    , authenticator_{}
    , userStore_{}
    , refreshToken_{}
{
    const auto user{LocalUserStore().user()};

    refreshToken_ = user.refreshToken();
    setHasPreviousLogin(!user.refreshToken().isEmpty());
}

void Login::authenticateCredentials()
{
    UserCredentail credentials{};
    credentials.setUsername(username_);
    credentials.setPassword(password_);
    if (!credentials.isValid())
    {
        emit errorOccurred();
        return;
    }

    // TODO: Think whether ResetUserAuthentication should be a QML_ELEMENT
    authenticator_ = new Data::RestUserAuthentication(credentials, this);
    authenticator_->authenticate();
    setMode(AuthenticationMode::LoginCredentials);

    const auto restAuth{static_cast<Data::RestUserAuthentication *>(authenticator_)};
    connect(restAuth, &Data::RestUserAuthentication::finished, this, [=, this] {
        setData(restAuth->data());
        const auto obj{QJsonDocument::fromJson(data_).object()};
        const QString codeString{obj.value("code").toString()};
        qDebug() << "Login::authenticateCredentials: codeString: " << codeString;
        if (codeString == "success")
        {
            save();
            setStatus(Status::Authenticated);
            emit authenticated();
        }
        else if (codeString == "invalidCredentials" || codeString == "notFound")
        {
            setStatus(Status::InvalidCredentials);
            emit errorOccurred();
        }
        else
        {
            setStatus(Status::Error);
            emit errorOccurred();
        }

        restAuth->deleteLater();
    });
}

void Login::authenticateRefreshToken()
{
    if (refreshToken_.isEmpty())
    {
        emit errorOccurred();
        return;
    }

    qDebug() << "Authenticationg user through LocalUserAuthentication...";
    authenticator_ = new Service::LocalUserAuthentication(this);
    authenticator_->authenticate();
    setMode(AuthenticationMode::RefreshToken);

    const auto localAuth{static_cast<Service::LocalUserAuthentication *>(authenticator_)};
    connect(localAuth, &Service::LocalUserAuthentication::finished, this, [=, this] {
        data_ = localAuth->buffer();
        qDebug() << "localAuth->isAuthenticated(): " << localAuth->isAuthenticated();
        if (localAuth->isAuthenticated())
        {
            restoreUser();
            const auto resp{QJsonDocument::fromJson(data_).object()};
            DataLayer::TokenManager::instance()->setAccessToken("Bearer " + resp["access_token"].toString());
            qDebug() << "User authenticated successfully through LocalUserAuthentication.";
            emit authenticated();
        }
        else
            emit errorOccurred();

        localAuth->deleteLater();
    });
}

void Login::save()
{
    if (!userStore_)
    {
        qWarning() << "None userStore was provided. User won't be stored.";
        return;
    }

    userStore_->save(data_);
}

void Login::setHasPreviousLogin(bool newHasPreviousLogin)
{
    if (hasPreviousLogin_ == newHasPreviousLogin)
        return;

    hasPreviousLogin_ = newHasPreviousLogin;
    emit hasPreviousLoginChanged();
}

void Login::restoreUser() const
{
    if (userStore_)
        userStore_->restore();
    else
        qDebug() << "Login::restoreUser: userStore is not set, the user cannot be restored.";
}

Login::AuthenticationMode Login::mode() const
{
    return mode_;
}

void Login::setMode(AuthenticationMode newMode)
{
    if (mode_ == newMode)
        return;
    mode_ = newMode;
    emit modeChanged();
}

QString Login::password() const
{
    return password_;
}

void Login::setPassword(const QString &newPassword)
{
    if (password_ == newPassword)
        return;
    password_ = newPassword;
    emit passwordChanged();
}

QString Login::username() const
{
    return username_;
}

void Login::setUsername(const QString &newUsername)
{
    if (username_ == newUsername)
        return;
    username_ = newUsername;
    emit usernameChanged();
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
