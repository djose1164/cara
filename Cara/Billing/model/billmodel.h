#ifndef BILLMODEL_H
#define BILLMODEL_H

#include "bill.h"
#include <QAbstractListModel>
#include <QtQml>

namespace Model {
class BillModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int count READ count NOTIFY countChanged FINAL)

public:
    enum class RoleNames
    {
        OrderIdRole = Qt::UserRole,
        DetailsRole,
        PaymentRole,
        DateRole,
        StatusRole,
        CustomerRole,
        TakenRole,
        DoneRole
    };

    explicit BillModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    QHash<int, QByteArray> roleNames() const override;

    int count() const;

    Q_INVOKABLE Entity::Bill *get(int index) const;

signals:
    void countChanged();

public slots:
    void insert(int index, Entity::Bill *const bill);
    void append(Entity::Bill *const bill);
    void remove(int index);
    void clear();

private:
    QList<Entity::Bill *> data_;
    QHash<int, QByteArray> roleNames_;
};
}

#endif // BILLMODEL_H
