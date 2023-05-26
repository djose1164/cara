#ifndef BILLMODEL_H
#define BILLMODEL_H

#include "billentity.h"
#include <QAbstractListModel>
#include <QtQml>

namespace ModelLayer {
using EntityLayer::BillEntity;

class BillModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    enum class RoleNames
    {
        OrderIdRole = Qt::UserRole,
        UserRole,
        DetailsRole,
        PaymentRole,
        DateRole
    };

    explicit BillModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    QHash<int, QByteArray> roleNames() const override;

    int count() const;

    Q_INVOKABLE BillEntity *get(int index) const;

signals:
    void countChanged();

public slots:
    void insert(int index, BillEntity *const bill);
    void append(BillEntity *const bill);
    void remove(int index);
    void clear();

private:
    QList<BillEntity *> data_;
    QHash<int, QByteArray> roleNames_;
};
}

#endif // BILLMODEL_H
