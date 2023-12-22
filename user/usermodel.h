#ifndef USERMODEL_H
#define USERMODEL_H

#include "userentity.h"
#include "userrepository.h"
#include <QAbstractListModel>
#include <QQmlEngine>

namespace Model {
class UserModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int count READ count NOTIFY countChanged FINAL)

public:
    enum class Roles
    {
        UsernameRole = Qt::UserRole,
        PasswordRole,
        ForenameRole,
        SurnameRole,
        CustomerIdRole,
        UserIdRole,
        UserTypeRole,
        isSalespersonRole,
        isAdminRole,
        isCustomerRole,
        isGuestRole,
        SalespersonRole,
    };
    Q_ENUM(Roles)

    explicit UserModel(QObject *parent = nullptr);

    int count() const;

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

public slots:
    void insert(int index, const Entity::UserEntity &user);

    void append(const Entity::UserEntity &user);

    void clear();

signals:
    void countChanged();

private:
    QList<Entity::UserEntity> data_;
    Data::UserRepository *repository_;

};
}
#endif // USERMODEL_H
