#ifndef PRODUCTMODEL_H
#define PRODUCTMODEL_H

#include "../Entities/productentity.h"
#include "iinventoryrepository.h"
#include <QAbstractListModel>
#include <QObject>
#include <QQmlEngine>

namespace Model
{
class ProductModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(Business::IInventoryRepository *repository READ repository WRITE setRepository NOTIFY repositoryChanged FINAL)
    Q_PROPERTY(int count READ count NOTIFY countChanged FINAL)
public:
    using SalespersonId = Business::IInventoryRepository::SalespersonId;
    enum class RoleNames
    {
        ProductIdRole = Qt::UserRole,
        ProductNameRole,
        BuyPriceRole,
        SellPriceRole,
        DescriptionRole,
        MaximumStockLevelRole,
        MinimumStockLevelRole,
        ReorderPointRole,
        QuantityAvailableRole,
        ImageUrlRole,
        SelectedQtyRole
    };
    explicit ProductModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    Business::IInventoryRepository *repository() const;
    void setRepository(Business::IInventoryRepository *newRepository);

    int count() const;

    Q_INVOKABLE QObject *get(int index) const;
    Q_INVOKABLE bool contains(const EntityLayer::Product *product);
    Q_INVOKABLE QModelIndex find(int productId);

public slots:
    void update(SalespersonId spId);
    void append(QObject * product);
    void clear();
    void remove(int index);

signals:
    void repositoryChanged();
    void countChanged();
    void updated();

private:
    QList<QPointer<EntityLayer::Product>> data_;
    Business::IInventoryRepository *repository_;
};
}

#endif // PRODUCTMODEL_H
