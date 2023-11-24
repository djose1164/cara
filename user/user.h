#ifndef ENTITY_USER_H
#define ENTITY_USER_H

#include <QObject>
#include <QQmlEngine>

namespace Entity {
class UserEntity
{
    Q_GADGET
    QML_NAMED_ELEMENT(UserEntity)
    QML_VALUE_TYPE(user)
    QML_UNCREATABLE("This is a gadget")
    QML_ELEMENT

    Q_PROPERTY(QString username READ username WRITE setUsername)
    Q_PROPERTY(QString password READ password WRITE setPassword)

    Q_PROPERTY(QString forename READ forename WRITE setForename)
    Q_PROPERTY(QString surname READ surname WRITE setSurname)
    Q_PROPERTY(QString fullname READ fullname CONSTANT)

    Q_PROPERTY(QString email READ email WRITE setEmail)

    Q_PROPERTY(UserTypes userType READ userType WRITE setUserType)

    Q_PROPERTY(int customerId READ customerId WRITE setCustomerId)
    Q_PROPERTY(int userId READ userId WRITE setUserId)
    Q_PROPERTY(int warehouseId READ warehouseId WRITE setWarehouseId)

public:
    enum class UserTypes {
        Guest,
        Admin = 1,
        User,
        Salesperson,
        Customer,
    };
    Q_ENUM(UserTypes)

    UserEntity();

    static UserEntity fromMap(const QVariantMap &map);

    bool operator==(const UserEntity &other);
    bool operator!=(const UserEntity &other);

    QString username() const;    
    QString password() const;

    QString forename() const;
    QString surname() const;
    QString fullname() const;

    QString email() const;
    void setEmail(const QString &email);

    UserTypes userType() const;

    int customerId() const;
    int userId() const;

    int warehouseId() const;
    void setWarehouseId(int newWarehouseId);

public slots:
    void setUsername(const QString &username);
    void setPassword(const QString &password);

    void setForename(const QString &forename);
    void setSurname(const QString &surname);

    void setUserType(UserTypes userType);

    void setCustomerId(int customerId);
    void setUserId(int userId);

    void clear();

private:
    QString username_;
    QString password_;

    QString forename_;
    QString surname_;

    QString email_;

    int customerId_;
    int userId_;
    int warehouseId_;

    UserTypes userType_;
};
}

#endif // ENTITY_USER_H
