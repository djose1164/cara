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

    Q_PROPERTY(QString forename READ surname WRITE setSurname NOTIFY surnameChanged)
    Q_PROPERTY(QString surname READ forename WRITE setForename NOTIFY forenameChanged)
    Q_PROPERTY(QString fullname READ fullname CONSTANT)

    Q_PROPERTY(UserTypes userType READ userType WRITE setUserType NOTIFY userTypeChanged)

public:
    enum class UserTypes {
        ADMIN = 1,
        USER,
        GUEST
    };
    Q_ENUM(UserTypes)

    explicit UserEntity(QObject *parent = nullptr);

    bool operator==(const UserEntity &other);
    bool operator!=(const UserEntity &other);

    QString username() const;    
    QString password() const;

    QString forename() const;
    QString surname() const;
    QString fullname() const;

    UserTypes userType() const;

public slots:
    void setUsername(const QString &username);
    void setPassword(const QString &password);

    void setForename(const QString &forename);
    void setSurname(const QString &surname);

    void setUserType(UserTypes userType);

signals:
    void usernameChanged();
    void passwordChanged();

    void forenameChanged();
    void surnameChanged();

    void userTypeChanged();

private:
    QString username_;
    QString password_;

    QString forename_;
    QString surname_;

    int customerId_;
    int userId_;

    UserTypes userType_;
};
}

#endif // USERENTITY_H
