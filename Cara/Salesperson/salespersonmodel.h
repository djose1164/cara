#ifndef SALESPERSONMODEL_H
#define SALESPERSONMODEL_H

#include "salesperson.h"
#include <QAbstractListModel>
#include <QObject>
#include <QQmlEngine>

namespace Model
{
class SalespersonModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int count READ count NOTIFY countChanged FINAL)
public:
    enum class Roles
    {
        CreditAvailableRole = Qt::UserRole,
        CreditConsumedRole,
        CreditLimitRole,
        SalespersonIdRole,
        SalespersonTypeRole,
        ForenameRole,
        SurnameRole,
        FullnameRole
    };

    explicit SalespersonModel(QObject *parent = nullptr);


    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;

    int count() const;

public slots:
    void insert(int index, const Entity::Salesperson &salesperson);
    void append(const Entity::Salesperson &salesperson);


signals:
    void countChanged();

private:
    QList<Entity::Salesperson> data_;
    int count_;
};
}

#endif // SALESPERSONMODEL_H
