#ifndef SQLITEPRODUCTMODEL_H
#define SQLITEPRODUCTMODEL_H

#include "abstractproductmodel.h"
#include <QSqlQueryModel>
#include <QQmlEngine>

namespace ProductInventory
{

class SQLiteProductModel : public QSqlQueryModel, public AbstractProductModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    using ProductId = int;

    enum class RoleNames
    {
        ProductIdRole = Qt::UserRole + 1,
        ProductNameRole,
        DescriptionRole,
        CategoryIdRole,
        PriceRole,
        BuyPriceRole,
        ImageUrlRole,
    };

    explicit SQLiteProductModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;

    void refresh();

    EntityLayer::Product getProductById(ProductId productId) const;

    // AbstractProductModel interface

    Q_INVOKABLE EntityLayer::Product get(int row) const override;
    Q_INVOKABLE bool contains(const EntityLayer::Product &product) const override;
    Q_INVOKABLE QModelIndex find(int productId) const override;
    QJsonArray toJsonArray() const override;

private:
    QModelIndex indexOf(int row, RoleNames role) const;
    QVariant valueAt(int row, RoleNames role) const;

    static int columnOf(RoleNames role);
};

} // namespace ProductInventory

#endif // SQLITEPRODUCTMODEL_H
