#ifndef ENTITY_USER_H
#define ENTITY_USER_H

#include "contact.h"
#include "userstore.h"
#include <QObject>
#include <QQmlEngine>
#include <QSettings>

namespace Controller
{
class User : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged FINAL)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged FINAL)

    Q_PROPERTY(QString forename READ forename WRITE setForename NOTIFY forenameChanged FINAL)
    Q_PROPERTY(QString surname READ surname WRITE setSurname NOTIFY surnameChanged FINAL)
    Q_PROPERTY(QString fullname READ fullname CONSTANT NOTIFY fullnameChanged FINAL)

    Q_PROPERTY(UserTypes userType READ userType WRITE setUserType NOTIFY userTypeChanged FINAL)

    Q_PROPERTY(int customerId READ customerId WRITE setCustomerId NOTIFY customerIdChanged FINAL)
    Q_PROPERTY(int userId READ userId WRITE setUserId NOTIFY userIdChanged FINAL)
    Q_PROPERTY(bool isSalesperson READ isSalesperson NOTIFY isSalespersonChanged FINAL)
    Q_PROPERTY(bool isAdmin READ isAdmin NOTIFY isAdminChanged FINAL)
    Q_PROPERTY(bool isUser READ isUser NOTIFY isUserChanged FINAL)
    Q_PROPERTY(Entity::Contact *contact READ contact WRITE setContact NOTIFY contactChanged FINAL)
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged FINAL)
public:
    enum class UserTypes {
        Guest,
        Admin = 1,
        User,
        Salesperson,
        Customer,
    };
    Q_ENUM(UserTypes)

    explicit User(QObject *parent = nullptr);

    static User *fromMap(const QVariantMap &map);


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

    bool isSalesperson() const;
    bool isAdmin() const;
    bool isUser() const;

    int contactId() const;
    void setContactId(int newContactId);

    Entity::Contact *contact() const;
    void setContact(Entity::Contact *newContact);

    QString errorString() const;
    void setErrorString(const QString &newErrorString);

public slots:
    void setUsername(const QString &username);
    void setPassword(const QString &password);

    void setForename(const QString &forename);
    void setSurname(const QString &surname);

    void setUserType(UserTypes userType);

    void setCustomerId(int customerId);
    void setUserId(int userId);

    void clear();

    void restore();

signals:
    void contactChanged();
    void usernameChanged();
    void passwordChanged();
    void forenameChanged();
    void surnameChanged();
    void fullnameChanged();
    void userTypeChanged();
    void userIdChanged();
    void isSalespersonChanged();
    void isAdminChanged();
    void isUserChanged();
    void customerIdChanged();

    void errorOccured();

    void errorStringChanged();

private:
    QString username_;
    QString password_;

    QString forename_;
    QString surname_;

    int customerId_;
    int userId_;

    UserTypes userType_;
    Entity::Contact *contact_;
    Business::UserStore *store_;

    QString errorString_;
};
}

#endif // ENTITY_USER_H
