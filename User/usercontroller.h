#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

#include "userrepository.h"
#include <QObject>
#include <QQmlEngine>

namespace Controller
{
class UserController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged FINAL)
public:
    using UserId = int;

    explicit UserController(QObject *parent = nullptr);

    Data::UserRepository *repository() const;
    void setRepository(Data::UserRepository *newRepository);

    QString errorString() const;
    void setErrorString(const QString &newErrorString);

    Q_INVOKABLE Entity::UserEntity create(const QVariantMap &data) const;

    Q_INVOKABLE Entity::UserEntity create(const QByteArray &data) const;

    Q_INVOKABLE Entity::UserEntity createEmpty() const;

public slots:
    void update(const QVariantMap &data);

    void getUserById(int id);

    void readFavorites(UserId id);

signals:
    void fetched(const QByteArray &data);
    void updated();
    void errorOccurred();

    void errorStringChanged();

private:
    Data::UserRepository *repository_;
    QString errorString_;
};
}

#endif // USERCONTROLLER_H
