#include "login.h"
#include "entity/usercredentail.h"
#include "data/restuserauthentication.h"
#include "service/localuserauthentication.h"
#include "../data/tokenmanager.h"
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
    refreshToken_ = settings.value("Credentials/refreshToken").toString();

    setHasPreviousLogin(!refreshToken_.isEmpty());

    connect(this, &Login::authenticated, this, [&] {
        const auto obj{QJsonDocument::fromJson(data_).object()};
        const QString token{"Bearer " + obj["access_token"].toString()};

        qDebug() << "Setting access token...";
        Q_ASSERT(!token.isEmpty());
        DataLayer::TokenManager::instance()->setAccessToken(token);
    });
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

    const auto restAuth{static_cast<Data::RestUserAuthentication *>(authenticator_)};
    connect(restAuth, &Data::RestUserAuthentication::finished, this, [=] {
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

    const auto localAuth{static_cast<Service::LocalUserAuthentication *>(authenticator_)};
    connect(localAuth, &Service::LocalUserAuthentication::finished, this, [=, this] {
        data_ = localAuth->buffer();
        qDebug() << "localAuth->isAuthenticated(): " << localAuth->isAuthenticated();
        if (localAuth->isAuthenticated()) {
            emit authenticated();
            QSettings settings;
            settings.setValue("Credentials/accessToken", DataLayer::TokenManager::instance()->accessToken());
            qDebug() << "User authenticated successfully through LocalUserAuthentication.";
        }

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
