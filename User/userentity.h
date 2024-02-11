#ifndef USERENTITY_H
#define USERENTITY_H

#include <QObject>
#include <QQmlEngine>

namespace Entity
{
class UserEntity
{
    Q_GADGET
    QML_VALUE_TYPE(user)
    Q_PROPERTY(QString username READ username WRITE setUsername FINAL)
    Q_PROPERTY(QString password READ password WRITE setPassword FINAL)
    Q_PROPERTY(QString forename READ forename WRITE setForename FINAL)
    Q_PROPERTY(QString surname READ surname WRITE setSurname FINAL)
    Q_PROPERTY(int customerId READ customerId WRITE setCustomerId FINAL)
    Q_PROPERTY(int userId READ userId WRITE setUserId FINAL)
    Q_PROPERTY(UserTypes userType READ userType WRITE setUserType FINAL)
    Q_PROPERTY(bool isSalesperson READ isSalesperson)
    Q_PROPERTY(bool isAdmin READ isAdmin)
    Q_PROPERTY(bool isCustomer READ isCustomer)
    Q_PROPERTY(int salespersonId READ salespersonId WRITE setSalespersonId FINAL)

public:
    using UserId = int;
    using SalespersonId = int;

    enum class UserTypes
    {
        Guest,
        Admin = 1,
        Customer,
        Salesperson,
    };
    Q_ENUM(UserTypes)
    
    static UserEntity fromJson(const QByteArray &data);

    UserEntity();

    bool operator==(const UserEntity &other);
    bool operator!=(const UserEntity &other);

    QString username() const;
    void setUsername(const QString &newUsername);

    QString password() const;
    void setPassword(const QString &newPassword);

    QString forename() const;
    void setForename(const QString &newForename);

    QString surname() const;
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

    SalespersonId salespersonId() const;
    void setSalespersonId(SalespersonId newSalespersonId);

private:
    QString username_;
    QString password_;

    QString forename_;
    QString surname_;

    int customerId_;
    UserId userId_;
    SalespersonId salespersonId_;

    UserTypes userType_;
};
}

namespace UserEntityForeign {
Q_NAMESPACE
QML_FOREIGN_NAMESPACE(Entity::UserEntity)
QML_NAMED_ELEMENT(UserEntity)
}

#endif // USERENTITY_H
