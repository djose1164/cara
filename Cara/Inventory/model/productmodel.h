#ifndef PRODUCTMODEL_H
#define PRODUCTMODEL_H

#include "productentity.h"
#include <QAbstractListModel>
#include <QObject>
#include <QQmlEngine>

namespace Business { class IInventoryRepository; }
namespace Model
{
class ProductModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(Business::IInventoryRepository *repository READ repository WRITE setRepository NOTIFY repositoryChanged FINAL)
    Q_PROPERTY(int count READ count NOTIFY countChanged FINAL)
public:
    using SalespersonId = int;
    using ProductId = int;

    enum class RoleNames
    {
        ProductIdRole = Qt::UserRole,
        ProductNameRole,
        PriceRole,
        DescriptionRole,
        MaximumStockLevelRole,
        MinimumStockLevelRole,
        ReorderPointRole,
        QuantityAvailableRole,
        ImageUrlRole,
        SelectedQtyRole,
        CategoryIdRole,
        ImagesRole,
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

    Q_INVOKABLE EntityLayer::Product get(int index) const;
    Q_INVOKABLE bool contains(const EntityLayer::Product &product) const;
    Q_INVOKABLE QModelIndex find(int productId) const;

public slots:
    void update(SalespersonId spId);
    void updateFromList(const QList<EntityLayer::Product> &products);
    void append(const EntityLayer::Product &product);
    void clear();
    void remove(int index);
    void removeByProductId(ProductId id);

signals:
    void repositoryChanged();
    void countChanged();
    void updated();

private:
    QList<EntityLayer::Product> data_;
    Business::IInventoryRepository *repository_;
};
}

#endif // PRODUCTMODEL_H
