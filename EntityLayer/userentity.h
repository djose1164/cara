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

public:
    explicit UserEntity(QObject *parent = nullptr);

    bool operator==(const UserEntity &other);
    bool operator!=(const UserEntity &other);

    void setUsername(const QString &username);
    QString username() const;

    void setPassword(const QString &password);
    QString password() const;

signals:
    void usernameChanged();
    void passwordChanged();

private:
    QString username_;
    QString password_;
};
}

#endif // USERENTITY_H
