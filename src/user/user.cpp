#include "user.h"
#include "localuserstore.h"
#include <QVariantMap>
#include <QJsonDocument>

namespace Controller
{
User::User(QObject *parent)
    : QObject{parent}
    , username_{}
    , password_{}
    , forename_{}
    , surname_{}
    , customerId_{}
    , userId_{}
    , userType_{UserTypes::Guest}
    , contact_{}
    , store_{new Business::LocalUserStore(this)}
{
}

User *User::fromMap(const QVariantMap &map)
{
    const auto contactDoc{map["contact"].toJsonDocument()};
    const auto contact{Entity::Contact::fromMap(contactDoc.toJson())};
    
    User *user{new User};
    user->setContact(contact);
    user->setForename(map["forename"].toString());
    user->setSurname(map["surname"].toString());
    user->setCustomerId(map["customer_id"].toInt());
    user->setUserType(static_cast<UserTypes>(map["user_type_id"].toInt()));
    user->setUserId(map["id"].toInt());
    user->setUsername(map["username"].toString());
    user->setCustomerId(map["customer_id"].toInt());

    return user;
}

void User::setUsername(const QString &username)
{
    if (username_ != username)
    {
        username_ = username;
        emit usernameChanged();
    }
}

QString User::username() const
{
    return username_;
}

void User::setPassword(const QString &password)
{
    if (password_ != password)
    {
        password_ = password;
        emit passwordChanged();
    }
}

void User::setForename(const QString &forename)
{
    if (forename_ != forename)
    {
        forename_ = forename;
        emit forenameChanged();
    }
}

void User::setSurname(const QString &surname)
{
    if (surname_ != surname)
    {
        surname_ = surname;
        emit surnameChanged();
    }
}

void User::setUserType(UserTypes userType)
{
    userType_ = userType;
}

void User::setCustomerId(int customerId)
{
    if (customerId_ != customerId)
    {
        customerId_ = customerId;
        emit customerIdChanged();
    }
}

void User::setUserId(int userId)
{
    if (userId_ != userId)
    {
        userId_ = userId;
    }
}

void User::clear()
{
    username_.clear();
    password_.clear();
    forename_.clear();
    surname_.clear();

    setCustomerId(0);
    setUserId(0);

    setUserType(UserTypes::Guest);
}

void User::restore()
{
    const auto restoredUser{store_->restore()};
    if (!restoredUser.has_value())
        return;

}

QString User::errorString() const
{
    return errorString_;
}

void User::setErrorString(const QString &newErrorString)
{
    if (errorString_ == newErrorString)
        return;
    errorString_ = newErrorString;
    emit errorStringChanged();
}

Entity::Contact *User::contact() const
{
    return contact_;
}

void User::setContact(Entity::Contact *newContact)
{
    if (contact_ == newContact)
        return;
    contact_ = newContact;
    emit contactChanged();
}

QString User::password() const
{
    return password_;
}

QString User::forename() const
{
    return forename_;
}

QString User::surname() const
{
    return surname_;
}

QString User::fullname() const
{
    return forename_ + " " + surname_;
}

User::UserTypes User::userType() const
{
    return userType_;
}

int User::customerId() const
{
    return customerId_;
}

int User::userId() const
{
    return userId_;
}

bool User::isSalesperson() const
{
    return userType_ == UserTypes::Salesperson;
}

bool User::isAdmin() const
{
    return userType_ == UserTypes::Admin;
}

bool User::isUser() const
{
    return userType_ == UserTypes::Customer;
}
}
