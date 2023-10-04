#ifndef USERENTITY_H
#define USERENTITY_H

#include <QObject>
#include <QtQml>

namespace EntityLayer {
class UserEntity : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)

    Q_PROPERTY(QString forename READ forename WRITE setForename NOTIFY forenameChanged)
    Q_PROPERTY(QString surname READ surname WRITE setSurname NOTIFY surnameChanged)
    Q_PROPERTY(QString fullname READ fullname CONSTANT)

    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY emailChanged)

    Q_PROPERTY(UserTypes userType READ userType WRITE setUserType NOTIFY userTypeChanged)

    Q_PROPERTY(int customerId READ customerId WRITE setCustomerId NOTIFY customerIdChanged)
    Q_PROPERTY(int userId READ userId WRITE setUserId NOTIFY userIdChanged)
    Q_PROPERTY(int warehouseId READ warehouseId WRITE setWarehouseId NOTIFY warehouseIdChanged)

public:
    enum class UserTypes {
        ADMIN = 1,
        USER,
        GUEST,
        CUSTOMER
    };
    Q_ENUM(UserTypes)

    explicit UserEntity(QObject *parent = nullptr);

    static UserEntity *fromMap(const QVariantMap &map);

    void fill(const QVariantMap &map);

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

signals:
    void usernameChanged();
    void passwordChanged();

    void forenameChanged();
    void surnameChanged();

    void emailChanged();

    void userTypeChanged();

    void userIdChanged();
    void customerIdChanged();

    void warehouseIdChanged();

private:
    QString username_;
    QString password_;

    QString forename_;
    QString surname_;

    QString email_;

    Q_OBJECT_BINDABLE_PROPERTY(UserEntity, int, customerId_, &UserEntity::customerIdChanged)
    int userId_;
    int warehouseId_;

    Q_OBJECT_BINDABLE_PROPERTY(UserEntity, UserTypes, userType_, &UserEntity::userTypeChanged)
};
}

#endif // USERENTITY_H
