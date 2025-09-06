#ifndef USERENTITY_H
#define USERENTITY_H

#include "address.h"
#include "ipersonname.h"
#include <QObject>
#include <QQmlEngine>

namespace Entity
{
class UserEntity : public IPersonName
{
    Q_GADGET
    QML_VALUE_TYPE(user)
    QML_STRUCTURED_VALUE
    Q_PROPERTY(QString username READ username WRITE setUsername FINAL)
    Q_PROPERTY(QString password READ password WRITE setPassword FINAL)
    Q_PROPERTY(QString email READ email WRITE setEmail FINAL)
    Q_PROPERTY(QString forename READ forename WRITE setForename FINAL)
    Q_PROPERTY(QString surname READ surname WRITE setSurname FINAL)
    Q_PROPERTY(QString fullname READ fullname FINAL)
    Q_PROPERTY(int customerId READ customerId WRITE setCustomerId FINAL)
    Q_PROPERTY(int userId READ userId WRITE setUserId FINAL)
    Q_PROPERTY(UserTypes userType READ userType WRITE setUserType FINAL)
    Q_PROPERTY(bool isSalesperson READ isSalesperson)
    Q_PROPERTY(bool isAdmin READ isAdmin)
    Q_PROPERTY(bool isCustomer READ isCustomer)
    Q_PROPERTY(int salespersonId READ salespersonId WRITE setSalespersonId FINAL)
    Q_PROPERTY(int sellerId READ salespersonId WRITE setSalespersonId FINAL)
    Q_PROPERTY(QString phoneNumber READ phoneNumber WRITE setPhoneNumber FINAL)
    Q_PROPERTY(QDate birthday READ birthday WRITE setBirthday FINAL)
    Q_PROPERTY(Address address READ address WRITE setAddress FINAL)
public:
    using UserId = int;
    using SellerId = int;

    enum class UserTypes
    {
        Guest,
        Admin,
        Customer,
        Salesperson,
    };
    Q_ENUM(UserTypes)
    
    [[deprecated("Use the overloaded version of QJsonObject instead.")]]
    static UserEntity fromJson(const QByteArray &data);

    UserEntity();

    bool operator==(const UserEntity &other);
    bool operator!=(const UserEntity &other);

    QString username() const;
    void setUsername(const QString &newUsername);

    QString password() const;
    void setPassword(const QString &newPassword);

    QString forename() const override;
    void setForename(const QString &newForename);

    QString surname() const override;
    void setSurname(const QString &newSurname);

    int customerId() const;
    void setCustomerId(int newCustomerId);

    UserId userId() const;
    void setUserId(UserId newUserId);

    UserTypes userType() const;
    void setUserType(UserTypes newUserType);

    bool isSalesperson() const;
    bool isAdmin() const;
    bool isCustomer() const;

    void clear();

    QString refreshToken() const;
    void setRefreshToken(const QString &newRefreshToken);

    QString accessToken() const;
    void setAccessToken(const QString &newAccessToken);

    SellerId salespersonId() const;
    void setSalespersonId(SellerId newSalespersonId);

    QString email() const;
    void setEmail(const QString &newEmail);

    QString phoneNumber() const;
    void setPhoneNumber(const QString &newPhoneNumber);

    QByteArray toJson() const;

    QDate birthday() const;
    void setBirthday(const QDate &newBirthday);

    Address address() const;
    void setAddress(const Address &newAddress);

    QString fullname() const override;

    static UserEntity fromJson(const QJsonObject &json);

private:
    QString username_;
    QString password_;
    QString email_;
    QString phoneNumber_;

    QString forename_;
    QString surname_;

    int customerId_;
    UserId userId_;
    SellerId sellerId_;

    UserTypes userType_;
    QDate birthday_;
    Address address_;
};

}

QDebug operator<<(QDebug dbg, const Entity::UserEntity &usr);

namespace UserEntityForeign
{
Q_NAMESPACE
QML_FOREIGN_NAMESPACE(Entity::UserEntity)
QML_NAMED_ELEMENT(UserEntity)
}

#endif // USERENTITY_H
