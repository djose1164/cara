#ifndef SELLERLISTMODEL_H
#define SELLERLISTMODEL_H

#include "abstractsellermodel.h"
#include "salesperson.h"
#include <QAbstractListModel>
#include <QObject>
#include <QQmlEngine>

namespace Seller
{
class SellerListModel : public QAbstractListModel, public Seller::AbstractSellerModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged FINAL)
    QML_ELEMENT
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
        FullnameRole,
        SellerIdRole
    };

    explicit SellerListModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;

    int count() const override;

public slots:
    // AbstractSellerModel interface
    void insert(int index, const Entity::Salesperson &salesperson) override;
    void append(const QVariantMap &salesperson) override;
    void setSellers(const QList<Entity::Salesperson> &sellerList) override;

signals:
    void countChanged();

private:
    QList<Entity::Salesperson> data_;
};
}

#endif // SELLERLISTMODEL_H
